#pragma once

#include <Algorithm/MCTS/mcts_node.hpp>
#include <Algorithm/MCTS/mcts_result.hpp>
#include <Algorithm/MCTS/heuristics.hpp>


namespace SpireSim {

    const int MCTS_POOL_RESERVE = 200000; 

    class MCTS_Pass : public Algorithm {
    public:
        UMCTS_Heuristic heuristic = nullptr;
        MCTS_ResultMap mcts_ResultMap;
        MCTSNodes nodePool;

        int optionIterations = 1000;
        double optionExplorationConstant = 1.414;

        int seedBuffer = 0;

        MCTS_Pass() {
            reserve();
        }

        MCTS_Pass(Combat *initialState_) : Algorithm(initialState_) {
            reserve();
        }

        MCTS_Pass(UMCTS_Heuristic heuristic_) : heuristic(std::move(heuristic_)) {
            reserve();
        }

        MCTS_Pass(Combat *initialState_, UMCTS_Heuristic heuristic_)
            : Algorithm(initialState_), heuristic(std::move(heuristic_))
        {
            reserve();
        }

        void reserve() {
            nodePool.reserve(MCTS_POOL_RESERVE);
        }

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
            int rootIndex = createNode(std::move(internalState));

            for(int i = 0; i < optionIterations; ++i) {
                int selected = select(rootIndex);

                int expanded = selected;
                if(!nodePool[expanded].state.isCombatOver()) expand(selected);

                auto [score, hpLoss] = rollout(expanded);
                backpropagate(expanded, score, hpLoss);
            }

            double bestActionScore = -99999;
            for(auto& childIndex : nodePool[rootIndex].childIndices) {
                auto& mcts_result = mcts_ResultMap[nodePool[childIndex].parentActionIndex];
                auto& resultScore = result.scoreMap[nodePool[childIndex].parentActionIndex];

                mcts_result.visits = nodePool[childIndex].visits;
                mcts_result.totalScore = nodePool[childIndex].totalScore;
                mcts_result.totalHpLoss = nodePool[childIndex].totalHpLoss;
                mcts_result.optionIterations = optionIterations;
                resultScore += nodePool[childIndex].visits;

                if(nodePool[childIndex].visits > bestActionScore) {
                    bestActionScore = nodePool[childIndex].visits;
                    bestActionIndex = nodePool[childIndex].parentActionIndex;
                }
            }

            clear();
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

        int select(int rootIndex) {
            int currentNodeIndex = rootIndex;

            while(nodePool[currentNodeIndex].untriedActionIndices.empty() &&
                    !nodePool[currentNodeIndex].childIndices.empty())
            {
                int bestChildIndex = -1;
                double bestUCB1 = -std::numeric_limits<double>::infinity();

                for (auto childIndex : nodePool[currentNodeIndex].childIndices) {
                    if(nodePool[childIndex].parentActionCardId != ENTITY_NONE)
                        cardStatsMap[nodePool[childIndex].parentActionCardId].countPlayable++;
                    double ucb1 = calculateUCB1(childIndex, nodePool[currentNodeIndex].visits, optionExplorationConstant);
                    if (ucb1 > bestUCB1) {
                        bestUCB1 = ucb1;
                        bestChildIndex = childIndex;
                    }
                }

                if (bestChildIndex == -1) break; 
                currentNodeIndex = bestChildIndex;

                if(nodePool[currentNodeIndex].parentActionCardId != ENTITY_NONE) {
                    auto& stats = cardStatsMap[nodePool[currentNodeIndex].parentActionCardId];
                    stats.countPlayedIfAble++;
                    if(nodePool[currentNodeIndex].visits > 0) stats.countPlayedIfAbleWeighted +=
                        (nodePool[currentNodeIndex].totalScore) / (nodePool[currentNodeIndex].visits);
                }
            }

            return currentNodeIndex;
        }

        int expand(int nodeIndex) {
            if (nodePool[nodeIndex].untriedActionIndices.empty()) {
                return nodeIndex; 
            }

            int actionCardEntityId = nodePool[nodeIndex].untriedActionIndices.back();
            int actionIndex = nodePool[nodeIndex].untriedActionIndices.size() - 1;
            nodePool[nodeIndex].untriedActionIndices.pop_back();

            Combat nextState = nodePool[nodeIndex].state;
            nextState.executeAction(actionIndex);

            int childIndex = createNode(std::move(nextState));
            nodePool[childIndex].parent = nodeIndex;
            nodePool[childIndex].parentActionIndex = actionIndex;
            nodePool[childIndex].parentActionCardId = actionCardEntityId;

            nodePool[nodeIndex].childIndices.push_back(childIndex);
            return childIndex;
        }

        std::pair<float, float> rollout(int nodeIndex, int maxSteps = 50) {
            auto stateCopy = nodePool[nodeIndex].state;
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

        void backpropagate(int nodeIndex, double score, double hpLoss) {
            int currentNodeIndex = nodeIndex;
            while(currentNodeIndex != -1) {
                nodePool[nodeIndex].visits++;
                nodePool[nodeIndex].totalScore += score;
                nodePool[nodeIndex].totalHpLoss += hpLoss;
                currentNodeIndex = nodePool[currentNodeIndex].parent;
            }
        }

        double calculateUCB1(int nodeIndex, int parentVisits, double explorationConstant = 1.414) {
            if (nodePool[nodeIndex].visits == 0) {
                return std::numeric_limits<double>::infinity(); // Priorize unvisited nodes
            }
            
            // Exploration vs exploitation
            return (nodePool[nodeIndex].totalScore / nodePool[nodeIndex].visits) + 
                explorationConstant * std::sqrt(std::log(parentVisits) / nodePool[nodeIndex].visits);
        }

        int createNode(Combat&& state) {
            nodePool.emplace_back(std::move(state));
            return nodePool.size() - 1;
        }

        void clear() {
            nodePool.clear();
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
