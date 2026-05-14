#pragma once

#include <Core/Combat/include.hpp>


namespace SpireSim {

    struct MCTSNode {
        CombatState state;
        int parentActionIndex = -1;
        MCTSNode* parent = nullptr;
        std::vector<std::unique_ptr<MCTSNode>> children;
        
        bool isExpanded = false;
        std::vector<int> untriedActionIndices;

        double visits = 0;
        double totalScore = 0;

        MCTSNode(const CombatState &state_) : state(state_) {
            for(int i = 0; i < state.getActions().size(); ++i) {
                untriedActionIndices.push_back(i);
            }
        }

        std::string toString(int padding = 0) {
            std::stringstream ss;
            for(int i = 0; i < padding; i++) ss << "  ";
            ss << parentActionIndex << "; visits: " << visits << "; totalScore: " << totalScore << "\n";
            for(auto& child : children) {
                ss << child->toString(padding + 1);
            }
            return ss.str();
        }

    };

}
