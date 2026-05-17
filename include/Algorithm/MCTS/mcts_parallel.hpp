#pragma once

#include <Algorithm/MCTS/mcts.hpp>


namespace SpireSim {

    class MCTS_Parallel {
    public:
    
        std::unique_ptr<MCTS_Heuristic> heuristic = nullptr;
        Combat *initialState = nullptr;
        int bestActionIndex = -1;

        std::map<int, MCTS_Result> results;

        int optionIterations = 1000;
        double optionExplorationConstant = 1.414;

        MCTS_Parallel() {}
        MCTS_Parallel(Combat *initialState_) : initialState(initialState_) {}

        void run() {
            int numThreads = std::thread::hardware_concurrency();
            int iterationsPerThread = optionIterations / numThreads;
            bestActionIndex = -1;

            assert(heuristic);
            assert(initialState);
            
            results.clear();
            std::vector<std::map<int, MCTS_Result>> threadResults(numThreads);
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
                    for(auto& [action, result] : mcts.results) {
                        auto& threadResult = threadResults[i][action];
                        threadResult.visits = result.visits;
                        threadResult.totalScore = result.totalScore;
                    }
                });
            }

            for(auto& t : workers) t.join();

            // Aggregiere die Ergebnisse: Addiere alle Visits über alle Threads hinweg
            for(auto& threadResult : threadResults) {
                for(auto const& [action, result] : threadResult) {
                    results[action].visits += result.visits;
                    results[action].totalScore += result.totalScore;
                }
            }
            
            // Die Aktion mit den meisten kumulierten Visits gewinnt
            double bestVisits = -99999;
            for(auto [action, result] : results) {
                if(result.visits > bestVisits) {
                    bestVisits = result.visits;
                    bestActionIndex = action;
                }
            }
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

        // std::string statsToString() {
        //     if(mapStats.empty()) return "No stats\n";
            
        //     std::stringstream ss;
        //     ss << "Stats:\n";

        //     for(auto& [cardEntityId, stats] : mapStats) {
        //         float played_perc = (stats.countPlayable > 0) ? (((double) stats.countPlayed) / ((double) stats.countPlayable)) : -1;
        //         float played_weighted_perc = (stats.countPlayable > 0) ?
        //             (((double) stats.countPlayedWeighted) / ((double) stats.countPlayable)) : -1;

        //         ss << "  Card " << cardEntityId
        //             << " (" << CardIdToString(initialState->getCardIdFromEntityId(cardEntityId)) << ")"
        //             << " seen: " << stats.countPlayable
        //             << " played: " << stats.countPlayed << "(" << played_perc << ")"
        //             << " played weighted: " << stats.countPlayedWeighted << "(" << played_weighted_perc << ")"
        //             << "\n";
        //     }
        //     return ss.str();
        // }

    };

}
