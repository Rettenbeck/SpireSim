#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    std::string Combat::stackToString() {
        std::stringstream ss;
        ss << "Stack: " << "\n";
        int counter = 0;
        for(auto& effect : state.stack) {
            ss << "  Pos " << counter << ": " << effect.toString() << "\n";
            counter++;
        }
        return ss.str();
    }
    
    std::string Combat::actionsToString() {
        std::stringstream ss;
        ss << "Actions: " << "\n";
        int counter = 0;
        for(auto& action : state.actions) {

            ss << counter << "  ";
            if(counter < 10) ss << " ";

            switch (action.actionType) {
                case ActionType::EndTurn:
                    ss << "End turn";
                    break;
                case ActionType::PlayCard:
                    ss << "Play card " << action.entityToPlay << " (";
                    ss << CardIdToString(ecs.getCard(action.entityToPlay).cardId);
                    ss << ") on " << action.targetEntityId;
                    break;
                default:
                    break;
            }
            ss << "\n";

            counter++;
        }
        return ss.str();
    }
    
    std::string Combat::toString() {
        std::stringstream ss;
        ss << "Ecs: " << ecs.toString() << "\n";
        ss << "PileHandler: " << pileHandler.toString() << "\n";
        ss << "CombatVariables: " << variables.toString() << "\n";
        ss << "EventRegistry: " << ToString(state.eventRegistry) << "\n";
        return ss.str();
    }
    
    std::string Combat::basicDataToString() {
        std::stringstream ss;
        ss << "Turn #" << variables.turn;
        ss << "; actions: " << getActions().size();
        ss << "; pHp: " << getPlayerHealth();
        ss << "; pBlock: " << getPlayerBlock();
        ss << "; eHp: " << getTotalEnemyHp();
        return ss.str();
    }

}
