#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    template<typename T>
    constexpr uint32_t COMB_EB(T t, bool flag) {
        return (static_cast<uint32_t>(t) << 1) | static_cast<uint32_t>(flag);
    }

    void Combat::playCard(Id cardEntityId, Id targetEntityId, bool force) {
        auto& card = ecs.getCard(cardEntityId);

        // Combine playableType and forceful play flag into one variable for fast switch
        uint32_t combined = COMB_EB(card.data.playableType, force);

        switch (combined) {
            case COMB_EB(PlayableType::Playable, false):
                variables.energy -= card.data.energyCost;
                variables.stars  -= card.data.starCost;
                break;
            case COMB_EB(PlayableType::Playable, true):
                break;
            case COMB_EB(PlayableType::XCost, false):
                variables.xValue = variables.energy;
                variables.energy = 0;
                break;
            case COMB_EB(PlayableType::XCost, true):
                variables.xValue = variables.energy;
                break;
            case COMB_EB(PlayableType::XCostStars, false):
                variables.xValue = variables.stars;
                variables.stars = 0;
                break;
            case COMB_EB(PlayableType::XCostStars, true):
                variables.xValue = variables.stars;
                break;
            default: break;
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
            case PlayableType::XCost:
                return true;
            case PlayableType::XCostStars:
                return true;
            default:
                assert(false);
        }
    }

    bool Combat::isCardPlayable(Id cardEntityId) {
        return isCardPlayable(ecs.getCard(cardEntityId));
    }

    Id Combat::chooseRandomEnemy() {
        if(ecs.enemyEntityIds.size() == 0) return ENTITY_NONE;
        auto randomId = state.rngCast.getRandomNumber(ecs.enemyEntityIds.size() - 1);
        assert(randomId < ecs.enemyEntityIds.size());
        return ecs.enemyEntityIds[randomId];
    }

}
