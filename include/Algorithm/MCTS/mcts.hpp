#pragma once

#include <Algorithm/MCTS/mcts_pass.hpp>


namespace SpireSim {

    class MCTS : public Algorithm {
    public:
        UMCTS_Heuristic heuristic = nullptr;
        MCTS_ResultMap mcts_ResultMap;

        int optionCombats = 100;
        int optionIterations = 1000;
        int optionNumberThreads = 1;
        double optionExplorationConstant = 1.414;

        MCTS() {}
        MCTS(Combat *initialState_) : Algorithm(initialState_) {}
        MCTS(Combat *initialState_, UMCTS_Heuristic heuristic_)
            : Algorithm(initialState_), heuristic(std::move(heuristic_)) {}

        void run() {
            cardStatsMap.clear();
            mcts_ResultMap.clear();
            result.clear();

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
                auto score = mcts_Result.getVisitsPerIteration();
                probabilities[actionIndex] = score;
                result.scoreMap[actionIndex] = score;

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

            if(numThreads > 1) {
                CardStatsMaps threadCardStats(numThreads);
                MCTS_ResultMaps threadResults(numThreads);
                std::vector<std::thread> workers;

                for(int i = 0; i < numThreads; ++i) {
                    workers.emplace_back([&, i]() {
                        auto pass = doPass(addedSeed + i * iterationsPerThread, iterationsPerThread);
                        threadCardStats[i] = pass->cardStatsMap;
                        threadResults[i] = pass->mcts_ResultMap;
                    });
                }

                for(auto& t : workers) t.join();

                Merge(sc_CardStats, threadCardStats);
                Merge(sc_ResultMap, threadResults);
            } else if(numThreads == 1) {
                auto pass = doPass(addedSeed, iterationsPerThread);
                sc_CardStats = pass->cardStatsMap;
                sc_ResultMap = pass->mcts_ResultMap;
            }
        }

        UMCTS_Pass doPass(int addedSeed, int iterationsPerThread) {
            auto pass = std::make_unique<MCTS_Pass>(initialState);
            pass->heuristic = heuristic->clone();
            pass->optionIterations = iterationsPerThread;
            pass->optionExplorationConstant = optionExplorationConstant;
            pass->optionAddedSeed = addedSeed;
            pass->heuristic->setSeed(pass->optionAddedSeed);
            pass->run();
            return std::move(pass);
        }

        UAlgorithm clone() {
            auto obj = std::make_unique<MCTS>(initialState, heuristic->clone());
            obj->optionCombats = optionCombats;
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

}
