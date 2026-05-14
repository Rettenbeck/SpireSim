#pragma once

#include <Algorithm/MCTS/mcts.hpp>


namespace SpireSim {

    class MCTS_Parallel {
    public:
    
        std::unique_ptr<MCTS_Heuristic> heuristic = nullptr;
        CombatState *initialState = nullptr;
        int bestActionIndex = -1;

        std::map<int, double> results;

        int optionIterations = 1000;
        double optionExplorationConstant = 1.414;

        MCTS_Parallel() {}
        MCTS_Parallel(CombatState *initialState_) : initialState(initialState_) {}

        void run() {
            int numThreads = std::thread::hardware_concurrency();
            int iterationsPerThread = optionIterations / numThreads;
            bestActionIndex = -1;

            assert(heuristic);
            assert(initialState);
            
            results.clear();
            std::vector<std::map<int, double>> threadResults(numThreads);
            std::vector<std::thread> workers;

            for(int i = 0; i < numThreads; ++i) {
                workers.emplace_back([&, i]() {
                    // Jede Instanz baut ihren eigenen Baum
                    MCTS mcts(initialState);
                    mcts.heuristic = heuristic->clone();
                    mcts.optionIterations = iterationsPerThread;
                    mcts.optionExplorationConstant = optionExplorationConstant;
                    mcts.optionAddedSeed = i * iterationsPerThread;
                    mcts.heuristic->setSeed(i * iterationsPerThread);
                    mcts.run();
                    
                    // Speichere die Visits der Kinder der Wurzel
                    for(auto& [action, visits] : mcts.results) {
                        threadResults[i][action] = visits;
                    }
                });
            }

            for(auto& t : workers) t.join();

            // Aggregiere die Ergebnisse: Addiere alle Visits über alle Threads hinweg
            for(auto& res : threadResults) {
                for(auto const& [action, visits] : res) {
                    results[action] += visits;
                }
            }
            
            // Die Aktion mit den meisten kumulierten Visits gewinnt
            double bestVisits = -99999;
            for(auto [action, visits] : results) {
                if(visits > bestVisits) {
                    bestVisits = visits;
                    bestActionIndex = action;
                }
            }
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
