#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::playCard(Id cardEntityId, Id targetEntityId, bool force) {
        auto& card = ecs.getCard(cardEntityId);
        variables.xValue = variables.energy;
        if(!force) {
            variables.energy -= card.data.energyCost;
        }
        card.targetEntityId = targetEntityId;
        moveCard(cardEntityId, CardLocation::Stack);
        putCardOntoStack(cardEntityId, targetEntityId);
    }
    
    void CombatState::forcePlayCard(Id cardEntityId, Id targetEntityId) {
        playCard(cardEntityId, targetEntityId, true);
    }
    
    void CombatState::resolveStackSingle() {
        if(stack.size() == 0) return;
        auto effect = stack[0];
        stack.erase(stack.begin());
        resolve(effect);
    }
    
    void CombatState::resolveStackFully() {
        while(stack.size() > 0 && waitingForActionOnStack == false && stopStack == false) {
            resolveStackSingle();
        }
    }

    bool CombatState::isCardPlayable(Card &card) {
        switch (card.data.playableType) {
            case PlayableType::Playable:
                return (card.data.energyCost <= variables.energy && card.data.starCost <= variables.stars);
            case PlayableType::Unplayable:
                return false;
            case PlayableType::Conditional:
                return false;
            default:
                assert(false);
        }
    }

    bool CombatState::isCardPlayable(Id cardEntityId) {
        return isCardPlayable(ecs.getCard(cardEntityId));
    }

}
