#pragma once

#include <Core/Combat/entity.hpp>


namespace SpireSim {

    struct MCTS_Result {
        double visits = 0;
        double totalScore = 0;
        double totalHpLoss = 0;
        int optionIterations = 0;

        float getVisitsPerIteration() {
            return (optionIterations > 0) ? (visits / ((double) optionIterations)) : 0;
        }

        float getScorePerVisit() {
            return (visits > 0) ? (totalScore / visits) : 0;
        }

        float getHpLossPerVisit() {
            return (visits > 0) ? (totalHpLoss / visits) : 0;
        }

        MCTS_Result& operator+=(const MCTS_Result& other) {
            visits += other.visits;
            totalScore += other.totalScore;
            totalHpLoss += other.totalHpLoss;
            optionIterations += other.optionIterations;
            return *this;
        }
    };
    using MCTS_ResultMap = std::map<int, MCTS_Result>;

    MCTS_ResultMap& Merge(MCTS_ResultMap& left, const MCTS_ResultMap& right) {
        for (const auto& [id, stats] : right) left[id] += stats;
        return left;
    }

    MCTS_ResultMap& Merge(MCTS_ResultMap& target, const std::vector<MCTS_ResultMap> &vec) {
        for(auto& map : vec) Merge(target, map);
        return target;
    }

    std::string ToString(MCTS_ResultMap &resultMap) {
        if(resultMap.empty()) return "No MCTS results\n";
        
        std::stringstream ss;
        ss << "MCTS Results:\n";

        for(auto [action, result] : resultMap) {
            ss << "  Action " << action << " has visited% : " << result.getVisitsPerIteration()
                << "; expected score: " << result.getScorePerVisit()
                << "; expected hp loss: " << result.getHpLossPerVisit()
                << "\n";

            // ss << action << "; " << result.visits << "; " << result.totalScore << "; " << result.optionIterations << "\n";
        }
        return ss.str();
    }

}
