#pragma once

#include <Core/Combat/Action/action_type.hpp>
#include <Core/Combat/entity.hpp>


namespace SpireSim {

    struct Action {
        ActionType actionType;
        Id entityToPlay;
        Id targetEntityId;
        
        Action() : actionType(ActionType::None), targetEntityId(ENTITY_NONE) {}
        Action(ActionType actionType_) : actionType(actionType_), targetEntityId(ENTITY_NONE) {}
        Action(ActionType actionType_, Id entityToPlay_) : actionType(actionType_), entityToPlay(entityToPlay_) {}
        Action(ActionType actionType_, Id entityToPlay_, Id targetEntityId_) :
            actionType(actionType_), entityToPlay(entityToPlay_), targetEntityId(targetEntityId_) {}
        
        std::string toString() {
            std::stringstream ss;
            ss << "actionType: " << ToString(actionType) << "; entityToPlay: " << entityToPlay << "; targetId: " << targetEntityId;
            return ss.str();
        }
    };
    using Actions = std::vector<Action>;

}
