#pragma once

#include <Algorithm/MCTS/mcts_node.hpp>
#include <Algorithm/MCTS/mcts_result.hpp>
#include <Algorithm/MCTS/heuristics.hpp>


namespace SpireSim {

    class MCTS_Pass : public Algorithm {
    public:
        UMCTS_Heuristic heuristic = nullptr;
        MCTS_ResultMap mcts_ResultMap;

        int optionIterations = 1000;
        double optionExplorationConstant = 1.414;

        int seedBuffer = 0;

        MCTS_Pass() {}
        MCTS_Pass(Combat *initialState_) : Algorithm(initialState_) {}
        MCTS_Pass(UMCTS_Heuristic heuristic_) : heuristic(std::move(heuristic_)) {}
        MCTS_Pass(Combat *initialState_, UMCTS_Heuristic heuristic_)
            : Algorithm(initialState_), heuristic(std::move(heuristic_)) {}

        void run() {
            assert(heuristic);
            assert(initialState);
            Combat internalState = *initialState;

            cardStatsMap.clear();
            result.clear();
            mcts_ResultMap.clear();

            internalState.increaseSeeds(optionAddedSeed);
            internalState.reshuffleDeck();
            // internalState.startCombat(true);
            MCTSNode root(internalState);

            for(int i = 0; i < optionIterations; ++i) {
                MCTSNode* selected = select(&root);

                MCTSNode* expanded = selected;
                if(!expanded->state.isCombatOver()) expand(selected);

                auto [score, hpLoss] = rollout(expanded);
                backpropagate(expanded, score, hpLoss);
            }

            double bestActionScore = -99999;
            for(auto& child : root.children) {
                auto& mcts_result = mcts_ResultMap[child->parentActionIndex];
                auto& resultScore = result.scoreMap[child->parentActionIndex];

                mcts_result.visits = child->visits;
                mcts_result.totalScore = child->totalScore;
                mcts_result.totalHpLoss = child->totalHpLoss;
                mcts_result.optionIterations = optionIterations;
                resultScore += child->visits;

                if(child->visits > bestActionScore) {
                    bestActionScore = child->visits;
                    bestActionIndex = child->parentActionIndex;
                }
            }
        }

        float evaluateState(Combat& state) {
            if(state.isCombatOver()) {
                if(state.isCombatVictorious()) {
                    float loss = ((float) state.getHpLoss()) / ((float) state.getPlayerMaxHealth());
                    return 1.0f - loss;
                } else {
                    float loss = ((float) state.getTotalEnemyHp()) / ((float) state.getTotalEnemyMaxHp());
                    return 0.f - loss;
                }
            }
            
            float loss = ((float) state.getHpLoss()) / ((float) state.getPlayerMaxHealth());
            return -loss;
        }

        MCTSNode* select(MCTSNode* root) {
            MCTSNode* current = root;

            while (current->untriedActionIndices.empty() && !current->children.empty()) {
                MCTSNode* bestChild = nullptr;
                double bestUCB1 = -std::numeric_limits<double>::infinity();

                for (auto& child : current->children) {
                    if(child->parentActionCardId != ENTITY_NONE) cardStatsMap[child->parentActionCardId].countPlayable++;
                    double ucb1 = calculateUCB1(*child, current->visits, optionExplorationConstant);
                    if (ucb1 > bestUCB1) {
                        bestUCB1 = ucb1;
                        bestChild = child.get();
                    }
                }

                if (bestChild == nullptr) break; 
                current = bestChild;

                if(current->parentActionCardId != ENTITY_NONE) {
                    auto& stats = cardStatsMap[current->parentActionCardId];
                    stats.countPlayedIfAble++;
                    if(current->visits > 0) stats.countPlayedIfAbleWeighted += (current->totalScore) / (current->visits);
                }
            }

            return current;
        }

        MCTSNode* expand(MCTSNode* node) {
            if (node->untriedActionIndices.empty()) {
                return node; // Nichts mehr zu tun, Knoten ist bereits voll expandiert
            }

            // 1. Wähle eine unversuchte Aktion (meistens die letzte, da effizienter beim Löschen)
            int actionCardEntityId = node->untriedActionIndices.back();
            int actionIndex = node->untriedActionIndices.size() - 1;
            node->untriedActionIndices.pop_back();

            // 2. Erzeuge einen neuen Combat durch Kopie und führe die Aktion aus
            Combat nextState = node->state; // Kopier-Konstruktor/Klonen
            nextState.executeAction(actionIndex);

            // 3. Erstelle den neuen Kind-Knoten
            auto child = std::make_unique<MCTSNode>(nextState);
            child->parent = node;
            child->parentActionIndex = actionIndex;
            child->parentActionCardId = actionCardEntityId;

            // 4. Füge das Kind der Liste hinzu und gib einen Pointer darauf zurück
            node->children.push_back(std::move(child));
            return node->children.back().get();
        }

        std::pair<float, float> rollout(MCTSNode* node, int maxSteps = 50) {
            auto stateCopy = node->state;
            stateCopy.increaseSeeds(seedBuffer++);
            stateCopy.reshuffleDeck();
            int steps = 0;
            while(!stateCopy.isCombatOver() && steps < maxSteps) {
                assert(!stateCopy.getActions().empty());
                int i = heuristic->getAction(stateCopy);
                assert(i >= 0);
                assert(i < stateCopy.getActions().size());
                stateCopy.executeAction(i);
            }
            return {evaluateState(stateCopy), stateCopy.getHpLoss()};
        }

        void backpropagate(MCTSNode* node, double score, double hpLoss) {
            MCTSNode* current = node;
            while (current != nullptr) {
                current->visits++;
                current->totalScore += score;
                current->totalHpLoss += hpLoss;
                current = current->parent;
            }
        }

        double calculateUCB1(const MCTSNode& node, int parentVisits, double explorationConstant = 1.414) {
            if (node.visits == 0) {
                return std::numeric_limits<double>::infinity(); // Unbesuchte Knoten priorisieren
            }
            // Ausbeutung (Exploitation) + Erkundung (Exploration)
            return (node.totalScore / node.visits) + 
                explorationConstant * std::sqrt(std::log(parentVisits) / node.visits);
        }
        
        std::unique_ptr<Algorithm> clone() {
            auto obj = std::make_unique<MCTS_Pass>(initialState, heuristic->clone());
            obj->optionIterations = optionIterations;
            obj->optionAddedSeed = optionAddedSeed;
            obj->optionExplorationConstant = optionExplorationConstant;
            return obj;
        }

        std::string toString() {
            std::stringstream ss;
            // ss << resultToString() << "\n";
            ss << "Best action index: " << bestActionIndex << "\n";
            ss << ToString(cardStatsMap) << "\n";
            ss << ToString(mcts_ResultMap) << "\n";
            return ss.str();
        }

    };
    using UMCTS_Pass = std::unique_ptr<MCTS_Pass>;

}
