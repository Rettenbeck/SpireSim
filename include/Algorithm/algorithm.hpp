#pragma once

#include <Core/Combat/include.hpp>
#include <Algorithm/card_stats.hpp>
#include <Algorithm/result.hpp>


namespace SpireSim {

    class Algorithm {
    public:
    
        Algorithm() {}
        Algorithm(Combat *initialState_) : initialState(initialState_) {}

        virtual void run() = 0;
        virtual std::unique_ptr<Algorithm> clone() = 0;
        
        Combat *initialState = nullptr;
        int optionAddedSeed = 0;

        CardStatsMap cardStatsMap;
        Result result;
        
        int bestActionIndex = -1;
        std::map<int, double> probabilities;

        std::string resultToString() {
            std::stringstream ss;
            ss << "Best action index: " << bestActionIndex << "\n";
            ss << "Action scores: \n";
            for(auto& [actionIndex, score] : result.scoreMap) {
                ss << "  Action " << actionIndex << ": " << score << "\n";
            }
            // ss << "Probabilites: \n";
            // for(auto& [actionIndex, probability] : probabilities) {
            //     ss << "  Action " << actionIndex << ": " << probability << "\n";
            // }
            return ss.str();
        }

    };

}
