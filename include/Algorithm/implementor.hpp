#pragma once

#include <Algorithm/algorithm.hpp>


namespace SpireSim {

    class Implementor : public Algorithm {
    public:
    
        UAlgorithm algorithm = nullptr;
        Combat *initialState = nullptr;

        int optionIterations = 100;
        int optionNumberThreads = 1;

        Implementor() {}
        Implementor(Combat *initialState_) : initialState(initialState_) {}
        Implementor(UAlgorithm algorithm_) : algorithm(std::move(algorithm_)) {}
        Implementor(Combat *initialState_, UAlgorithm algorithm_)
            : initialState(initialState_), algorithm(std::move(algorithm_)) {}

        void run() {
            assert(initialState);
            assert(algorithm);
            cardStatsMap.clear();
            result.clear();
            algorithm->initialState = initialState;

            runSingleCombat(cardStatsMap, result, optionAddedSeed);

            double bestActionScore = -99999;
            for(auto& [actionIndex, score] : result.scoreMap) {
                score /= optionIterations;
                if(score > bestActionScore) {
                    bestActionScore = score;
                    bestActionIndex = actionIndex;
                }
            }
        }

        void runSingleCombat(CardStatsMap &sc_CardStats, Result &sc_ResultMap, int addedSeed = 0) {
            assert(optionNumberThreads != 0);
            int numThreads = (optionNumberThreads == -1) ? std::thread::hardware_concurrency() : optionNumberThreads;
            int iterationsPerThread = optionIterations / numThreads;

            assert(algorithm);
            assert(initialState);
            sc_ResultMap.clear();

            if(numThreads > 1) {
                CardStatsMaps threadCardStats(numThreads);
                Results threadResults(numThreads);
                std::vector<std::thread> workers;

                for(int i = 0; i < numThreads; ++i) {
                    workers.emplace_back([&, i]() {
                        auto pass = doPass(addedSeed + i * iterationsPerThread, iterationsPerThread);
                        threadCardStats[i] = pass->cardStatsMap;
                        threadResults[i] = pass->result;
                    });
                }

                for(auto& t : workers) t.join();

                Merge(sc_CardStats, threadCardStats);
                Merge(sc_ResultMap, threadResults);
            } else if(numThreads == 1) {
                auto pass = doPass(addedSeed, iterationsPerThread);
                sc_CardStats = pass->cardStatsMap;
                sc_ResultMap = pass->result;
            }
        }

        UAlgorithm doPass(int addedSeed, int iterationsPerThread) {
            auto pass = algorithm->clone();
            pass->optionAddedSeed = addedSeed;
            pass->run();
            return std::move(pass);
        }

        UAlgorithm clone() {
            auto obj = std::make_unique<Implementor>(initialState, algorithm->clone());
            obj->optionIterations = optionIterations;
            obj->optionAddedSeed = optionAddedSeed;
            obj->optionNumberThreads = optionNumberThreads;
            return obj;
        }

        std::string toString() {
            std::stringstream ss;
            ss << resultToString() << "\n";
            ss << ToString(cardStatsMap) << "\n";
            return ss.str();
        }

    };
    using UImplementor = std::unique_ptr<Implementor>;

}
