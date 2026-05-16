#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    void Combat::generateActions() {
        state.actions.clear();
        if(state.combatOver) return;
        state.actions.push_back(Action(ActionType::EndTurn));
        for(auto& cardId : pileHandler.hand) {
            auto& card = ecs.getCard(cardId);
            if(!isCardPlayable(card)) continue;
            switch(card.data.targetingType) {
                case TargetingType::Single:
                    for(auto& enemyId : ecs.enemyEntityIds) {
                        state.actions.push_back(Action(ActionType::PlayCard, cardId, enemyId));
                    }
                    break;
                default:
                    state.actions.push_back(Action(ActionType::PlayCard, cardId));
                    break;
            }
        }
    }

    void Combat::executeAction(int actionIndex) {
        assert(actionIndex < state.actions.size());
        auto& action = state.actions[actionIndex];
        switch (action.actionType) {
            case ActionType::EndTurn:
                state.waitingForAction = false;
                break;
            case ActionType::PlayCard:
                playCard(action.entityToPlay, action.targetEntityId);
                resolveStackFully();
                break;
            default:
                break;
        }
        if(state.waitingForAction) {
            generateActions();
        } else {
            proceedPhases();
        }
    }

    void Combat::startCombat(bool shuffleDeck) {
        if(state.turnId == TurnId::None) {
            if(shuffleDeck) pileHandler.reshuffleDeck(ecs);
            proceedPhases();
        }
    }

    void Combat::reshuffleDeck() {
        pileHandler.reshuffleDeck(ecs);
    }

}
