#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::generateActions() {
        actions.clear();
        if(combatOver) return;
        actions.push_back(Action(ActionType::EndTurn));
        for(auto& cardId : pileHandler.hand) {
            auto& card = ecs.getCard(cardId);
            if(!isCardPlayable(card)) continue;
            switch(card.data.targetingType) {
                case TargetingType::Single:
                    for(auto& enemyId : ecs.enemyEntityIds) {
                        actions.push_back(Action(ActionType::PlayCard, cardId, enemyId));
                    }
                    break;
                default:
                    actions.push_back(Action(ActionType::PlayCard, cardId));
                    break;
            }
        }
    }

    void CombatState::executeAction(int actionIndex) {
        assert(actionIndex < actions.size());
        auto& action = actions[actionIndex];
        switch (action.actionType) {
            case ActionType::EndTurn:
                waitingForAction = false;
                break;
            case ActionType::PlayCard:
                playCard(action.entityToPlay, action.targetEntityId);
                resolveStackFully();
                break;
            default:
                break;
        }
        if(waitingForAction) {
            generateActions();
        } else {
            proceedPhases();
        }
    }

    void CombatState::startCombat(bool shuffleDeck) {
        if(turnId == TurnId::None) {
            if(shuffleDeck) pileHandler.reshuffleDeck(ecs);
            variables.initialHp = ecs.getPlayer().data.hp;
            proceedPhases();
        }
    }

    void CombatState::reshuffleDeck() {
        pileHandler.reshuffleDeck(ecs);
    }

}
