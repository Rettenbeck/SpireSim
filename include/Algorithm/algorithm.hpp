#pragma once

#include <Core/Combat/include.hpp>
#include <Algorithm/card_stats.hpp>
#include <Algorithm/result.hpp>


namespace SpireSim {

    class Algorithm {
    public:
        //
        Algorithm() {}

        virtual void run() = 0;
        
        Combat *initialState = nullptr;

        CardStatsMap cardStatsMap;
        Result result;
        
        int bestActionIndex = -1;
        std::map<int, double> probabilities;

        std::string resultToString() {
            std::stringstream ss;
            ss << "Best action index: " << bestActionIndex << "\n";
            ss << "Probabilites: \n";
            for(auto& [actionIndex, probability] : probabilities) {
                ss << "  Action " << actionIndex << ": " << probability << "\n";
            }
            return ss.str();
        }

    };

}
