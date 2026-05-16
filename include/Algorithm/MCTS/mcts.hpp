#pragma once

#include <Algorithm/MCTS/mcts_node.hpp>
#include <Algorithm/MCTS/heuristics.hpp>


namespace SpireSim {

    class MCTS {
    public:
        const float MAX_SCORE = 1000.0f;

        std::unique_ptr<MCTS_Heuristic> heuristic = nullptr;
        Combat *initialState = nullptr;
        int bestActionIndex = -1;

        std::map<int, double> results;

        int optionIterations = 1000;
        double optionExplorationConstant = 1.414;
        int optionAddedSeed = 0;

        int seedBuffer = 0;

        MCTS() {}
        MCTS(Combat *initialState_) : initialState(initialState_) {}

        void run() {
            assert(heuristic);
            assert(initialState);

            results.clear();
            bestActionIndex = -1;
            Combat internalState = *initialState;

            internalState.increaseSeeds(optionAddedSeed);
            internalState.startCombat(true);
            MCTSNode root(internalState);

            for(int i = 0; i < optionIterations; ++i) {
                MCTSNode* selected = select(&root);
                // std::cout << "[MCTS] Selected: " << selected << "\n";

                MCTSNode* expanded = selected;
                if(!expanded->state.isCombatOver()) expand(selected);
                // std::cout << "[MCTS] Expanded: " << expanded << "\n";

                float score = rollout(expanded);
                // std::cout << "[MCTS] Rollout score: " << score << "\n";
                backpropagate(expanded, score);
            }

            //std::cout << root.toString() << "\n";
            for(auto& child : root.children) {
                results[child->parentActionIndex] = child->visits;
            }

            bestActionIndex = getBestActionIndex(root);
        }

        float evaluateState(Combat& state) {
            if(state.isCombatOver()) {
                if(state.isCombatVictorious()) {
                    float loss = ((float) state.getHpLoss()) / ((float) state.getPlayerMaxHealth());
                    return 1.0f - loss;
                } else {
                    return -1.0f;
                }
            }
            
            float loss = ((float) state.getHpLoss()) / ((float) state.getPlayerMaxHealth());

            // float score = 0;
            // score += state.getPlayerHealth() * 2.0f;
            // score += state.getPlayerBlock () * 1.0f;
            // score -= state.getTotalEnemyHp() * 1.5f;

            // if(score > (MAX_SCORE - 1)) score = MAX_SCORE - 1;
            // if(score < (1 - MAX_SCORE)) score = 1 - MAX_SCORE;

            return -loss;
        }

        MCTSNode* select(MCTSNode* root) {
            MCTSNode* current = root;

            while (current->untriedActionIndices.empty() && !current->children.empty()) {
                MCTSNode* bestChild = nullptr;
                double bestUCB1 = -std::numeric_limits<double>::infinity();

                for (auto& child : current->children) {
                    double ucb1 = calculateUCB1(*child, current->visits, optionExplorationConstant);
                    if (ucb1 > bestUCB1) {
                        bestUCB1 = ucb1;
                        bestChild = child.get();
                    }
                }

                if (bestChild == nullptr) break; 
                current = bestChild;
            }

            return current;
        }

        MCTSNode* expand(MCTSNode* node) {
            if (node->untriedActionIndices.empty()) {
                return node; // Nichts mehr zu tun, Knoten ist bereits voll expandiert
            }

            // 1. Wähle eine unversuchte Aktion (meistens die letzte, da effizienter beim Löschen)
            int actionIndex = node->untriedActionIndices.back();
            node->untriedActionIndices.pop_back();

            // 2. Erzeuge einen neuen Combat durch Kopie und führe die Aktion aus
            Combat nextState = node->state; // Kopier-Konstruktor/Klonen
            nextState.executeAction(actionIndex);

            // 3. Erstelle den neuen Kind-Knoten
            auto child = std::make_unique<MCTSNode>(nextState);
            child->parent = node;
            child->parentActionIndex = actionIndex;

            // 4. Füge das Kind der Liste hinzu und gib einen Pointer darauf zurück
            node->children.push_back(std::move(child));
            return node->children.back().get();
        }

        float rollout(MCTSNode* node, int maxSteps = 50) {
            auto stateCopy = node->state;
            stateCopy.increaseSeeds(seedBuffer++);
            stateCopy.reshuffleDeck();
            int steps = 0;
            while(!stateCopy.isCombatOver() && steps < maxSteps) {
                assert(!stateCopy.getActions().empty());
                int i = heuristic->getAction(stateCopy);
                stateCopy.executeAction(i);
            }
            return evaluateState(stateCopy);
        }

        void backpropagate(MCTSNode* node, double score) {
            MCTSNode* current = node;
            while (current != nullptr) {
                current->visits++;
                current->totalScore += score;
                current = current->parent;
            }
        }

        int getBestActionIndex(const MCTSNode& root) {
            int bestAction = -1;
            double maxVisits = -1;

            for (const auto& child : root.children) {
                if (child->visits > maxVisits) {
                    maxVisits = child->visits;
                    bestAction = child->parentActionIndex;
                }
            }
            return bestAction;
        }

        double calculateUCB1(const MCTSNode& node, int parentVisits, double explorationConstant = 1.414) {
            if (node.visits == 0) {
                return std::numeric_limits<double>::infinity(); // Unbesuchte Knoten priorisieren
            }
            // Ausbeutung (Exploitation) + Erkundung (Exploration)
            return (node.totalScore / node.visits) + 
                explorationConstant * std::sqrt(std::log(parentVisits) / node.visits);
        }
        
        std::string resultsToString() {
            if(results.empty()) return "No results\n";
            
            std::stringstream ss;
            ss << "Results:\n";

            for(auto [action, visits] : results) {
                ss << "  Action " << action << " has score: " << (visits / (double) optionIterations) << "\n";
            }
            return ss.str();
        }

    };

}
