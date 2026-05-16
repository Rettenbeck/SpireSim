#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    void Combat::playCard(Id cardEntityId, Id targetEntityId, bool force) {
        auto& card = ecs.getCard(cardEntityId);
        variables.xValue = variables.energy;
        if(!force) {
            variables.energy -= card.data.energyCost;
        }
        card.targetEntityId = targetEntityId;
        moveCard(cardEntityId, CardLocation::Stack);
        putCardOntoStack(cardEntityId, targetEntityId);
    }
    
    void Combat::forcePlayCard(Id cardEntityId, Id targetEntityId) {
        playCard(cardEntityId, targetEntityId, true);
    }
    
    bool Combat::isCardPlayable(Card &card) {
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

    bool Combat::isCardPlayable(Id cardEntityId) {
        return isCardPlayable(ecs.getCard(cardEntityId));
    }

}
