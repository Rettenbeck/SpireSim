#pragma once

#include <Core/Combat/include.hpp>
#include <Algorithm/stats.hpp>


namespace SpireSim {

    struct MCTSNode {
        Combat state;
        int parentActionIndex = -1;
        Id parentActionCardId = ENTITY_NONE;
        MCTSNode* parent = nullptr;
        std::vector<std::unique_ptr<MCTSNode>> children;
        
        bool isExpanded = false;
        std::vector<Id> untriedActionIndices;

        double visits = 0;
        double totalScore = 0;

        Id cardEntityIdToPlay = ENTITY_NONE;

        MCTSNode(const Combat &state_) : state(state_) {
            for(auto& action : state.getActions()) {
                if(action.actionType == ActionType::PlayCard) {
                    untriedActionIndices.push_back(action.entityToPlay);
                } else {
                    untriedActionIndices.push_back(ENTITY_NONE);
                }
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
