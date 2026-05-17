#pragma once

#include <Algorithm/MCTS/mcts_pass.hpp>


namespace SpireSim {

    class MCTS : public Algorithm {
    public:
    
        std::unique_ptr<MCTS_Heuristic> heuristic = nullptr;
        Combat *initialState = nullptr;

        MCTS_ResultMap mcts_ResultMap;

        int optionCombats = 100;
        int optionIterations = 1000;
        int optionAddedSeed = 0;
        int optionNumberThreads = -1;
        double optionExplorationConstant = 1.414;

        MCTS() {}
        MCTS(Combat *initialState_) : initialState(initialState_) {}

        void run() {
            cardStatsMap.clear();
            mcts_ResultMap.clear();

            bestActionIndex = -1;
            probabilities.clear();

            for(int i = 0; i < optionCombats; i++) {
                CardStatsMap sc_CardStatsMap;
                MCTS_ResultMap sc_ResultMap;
                int addedSeed = optionAddedSeed + i * optionIterations;

                runSingleCombat(sc_CardStatsMap, sc_ResultMap, addedSeed);

                Merge(cardStatsMap, sc_CardStatsMap);
                Merge(mcts_ResultMap, sc_ResultMap);
            }

            double bestActionScore = -99999;
            for(auto& [actionIndex, mcts_Result] : mcts_ResultMap) {
                probabilities[actionIndex] = mcts_Result.getVisitsPerIteration();

                if(mcts_Result.visits > bestActionScore) {
                    bestActionScore = mcts_Result.visits;
                    bestActionIndex = actionIndex;
                }
            }
        }

        void runSingleCombat(CardStatsMap &sc_CardStats, MCTS_ResultMap &sc_ResultMap, int addedSeed = 0) {
            assert(optionNumberThreads != 0);
            int numThreads = (optionNumberThreads == -1) ? std::thread::hardware_concurrency() : optionNumberThreads;
            int iterationsPerThread = optionIterations / numThreads;

            assert(heuristic);
            assert(initialState);

            sc_ResultMap.clear();
            std::vector<CardStatsMap> threadCardStats(numThreads);
            std::vector<MCTS_ResultMap> threadResults(numThreads);
            std::vector<std::thread> workers;

            for(int i = 0; i < numThreads; ++i) {
                workers.emplace_back([&, i]() {
                    MCTS_Pass pass(initialState);
                    pass.heuristic = heuristic->clone();
                    pass.optionIterations = iterationsPerThread;
                    pass.optionExplorationConstant = optionExplorationConstant;
                    pass.optionAddedSeed = addedSeed + i * iterationsPerThread;
                    pass.heuristic->setSeed(pass.optionAddedSeed);
                    pass.run();
                    
                    for(auto& [cardEntityId, stats] : pass.cardStatsMap) {
                        threadCardStats[i][cardEntityId] = stats;
                    }
                    for(auto& [actionIndex, passResult] : pass.mcts_ResultMap) {
                        threadResults[i][actionIndex] = passResult;
                    }
                });
            }

            for(auto& t : workers) t.join();

            Merge(sc_CardStats, threadCardStats);
            Merge(sc_ResultMap, threadResults);
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

}
