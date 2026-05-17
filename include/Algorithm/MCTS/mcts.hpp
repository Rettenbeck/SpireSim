#pragma once

#include <Algorithm/MCTS/mcts_node.hpp>
#include <Algorithm/MCTS/heuristics.hpp>
#include <Algorithm/MCTS/mcts_result.hpp>


namespace SpireSim {

    class MCTS {
    public:
        const float MAX_SCORE = 1000.0f;

        std::unique_ptr<MCTS_Heuristic> heuristic = nullptr;
        Combat *initialState = nullptr;
        int bestActionIndex = -1;

        std::map<int, MCTS_Result> results;
        std::map<Id, Stats> mapStats;

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

            mapStats.clear();

            internalState.increaseSeeds(optionAddedSeed);
            internalState.startCombat(true);
            MCTSNode root(internalState);

            for(int i = 0; i < optionIterations; ++i) {
                MCTSNode* selected = select(&root);

                MCTSNode* expanded = selected;
                if(!expanded->state.isCombatOver()) expand(selected);

                float score = rollout(expanded);
                backpropagate(expanded, score);
            }

            for(auto& child : root.children) {
                auto& result = results[child->parentActionIndex];
                result.visits = child->visits;
                result.totalScore = child->totalScore;
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
            return -loss;
        }

        MCTSNode* select(MCTSNode* root) {
            MCTSNode* current = root;

            while (current->untriedActionIndices.empty() && !current->children.empty()) {
                MCTSNode* bestChild = nullptr;
                double bestUCB1 = -std::numeric_limits<double>::infinity();

                for (auto& child : current->children) {
                    if(child->parentActionCardId != ENTITY_NONE) mapStats[child->parentActionCardId].countPlayable++;
                    double ucb1 = calculateUCB1(*child, current->visits, optionExplorationConstant);
                    if (ucb1 > bestUCB1) {
                        bestUCB1 = ucb1;
                        bestChild = child.get();
                    }
                }

                if (bestChild == nullptr) break; 
                current = bestChild;

                if(current->parentActionCardId != ENTITY_NONE) {
                    auto& stats = mapStats[current->parentActionCardId];
                    stats.countPlayed++;
                    if(current->visits > 0) stats.countPlayedWeighted += (current->totalScore) / (current->visits);
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

            for(auto [action, result] : results) {
                float score_per_visit = (result.visits > 0) ? (result.totalScore / result.visits) : -1;
                float visits_per_iteration = (optionIterations > 0) ? (result.visits / (double) optionIterations) : -1;

                ss << "  Action " << action << " has visited% : " << visits_per_iteration
                    << "; expected score: " << score_per_visit
                    << "\n";
            }
            return ss.str();
        }

        std::string statsToString() {
            if(mapStats.empty()) return "No stats\n";
            
            std::stringstream ss;
            ss << "Stats:\n";

            for(auto& [cardEntityId, stats] : mapStats) {
                float played_perc = (stats.countPlayable > 0) ? (((double) stats.countPlayed) / ((double) stats.countPlayable)) : -1;
                float played_weighted_perc = (stats.countPlayable > 0) ?
                    (((double) stats.countPlayedWeighted) / ((double) stats.countPlayable)) : -1;

                ss << "  Card " << cardEntityId
                    << " (" << CardIdToString(initialState->getCardIdFromEntityId(cardEntityId)) << ")"
                    << " seen: " << stats.countPlayable
                    << "; played: " << stats.countPlayed << " (" << played_perc << ")"
                    << "; played weighted: " << stats.countPlayedWeighted << " (" << played_weighted_perc << ")"
                    << "\n";
            }
            return ss.str();
        }

    };

}
