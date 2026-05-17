#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    struct Result {
        std::map<int, double> scoreMap;
        
        Result& operator+=(const Result& other) {
            for (const auto& [index, score] : other.scoreMap) this->scoreMap[index] += score;
            return *this;
        }

        void clear() {
            scoreMap.clear();
        }

        std::string toString() {
            std::stringstream ss;
            ss << "Results:\n";
            for(auto [index, score] : scoreMap) {
                ss << "  Action " << index << " has score : " << score << "\n";
            }
            return ss.str();
        }

    };

}
