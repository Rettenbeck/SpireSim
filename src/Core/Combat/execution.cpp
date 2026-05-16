#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    void Combat::executeUnpackCard(Effect &effect) {
        auto& card = ecs.getCard(effect.sourceEntityId);
        int i = 0;
        for(int j = 0; j < card.data.replay + 1; j++) {
            for(auto& effect_ : card.data.effects) {
                effect_.sourceEntityId = effect.sourceEntityId;
                putEffectOntoStack(effect_, i++);
            }
        }
        state.stack.insert(state.stack.begin() + i, Effect(EffectType::FinishCardPlayed, {}, effect.sourceEntityId));
    }
    
    void Combat::executeFinishCardPlayed(Effect &effect) {
        auto& card = ecs.getCard(effect.sourceEntityId);
        moveCard(effect.sourceEntityId, card.data.pileAfterPlay);
        variables.cardsPlayedThisCombat++;
        triggerEvent(EventType::OnCardPlayed);
    }

    void Combat::executeDrawCards(Effect &effect) {
        assert(effect.resolutionParams.size() > 0);
        drawCards(effect.resolutionParams[0]);
    }
    
    void Combat::executeCardDealDamage(Effect &effect) {
        assert(effect.resolutionParams.size() > 0);
        auto& card = ecs.getCard(effect.sourceEntityId);
        auto& cardData = cardPool.retrieveData(card);
        int& damageToDeal = effect.resolutionParams[0];

        switch(cardData.targetingType) {
            case TargetingType::Single:
                dealDamageToEnemy(ecs.playerEntityId, ecs.getPlayer().data, card.targetEntityId, damageToDeal);
                break;
            case TargetingType::All:
                for(auto id : ecs.enemyEntityIds) {
                    dealDamageToEnemy(ecs.playerEntityId, ecs.getPlayer().data, id, damageToDeal);
                }
                break;
            default:
                assert(false);
        }
    }
    
    void Combat::executeCardGainBlock(Effect &effect) {
        assert(effect.resolutionParams.size() > 0);
        auto& card = ecs.getCard(effect.sourceEntityId);
        auto& cardData = cardPool.retrieveData(card);
        int& blockToGain = effect.resolutionParams[0];
        gainBlock(ecs.playerEntityId, blockToGain);
    }
    
    void Combat::executeCardApplyVulnerable(Effect &effect) {
        assert(effect.resolutionParams.size() > 0);
        auto& card = ecs.getCard(effect.sourceEntityId);
        auto& cardData = cardPool.retrieveData(card);
        int& vulnerableToApply = effect.resolutionParams[0];

        switch(cardData.targetingType) {
            case TargetingType::Single:
                applyVulnerableToEnemy(card.targetEntityId, vulnerableToApply);
                break;
            case TargetingType::All:
                for(auto id : ecs.enemyEntityIds) {
                    applyVulnerableToEnemy(id, vulnerableToApply);
                }
                break;
            default:
                assert(false);
        }
    }
    
    void Combat::executeMoveCard(Effect &effect) {
        assert(effect.resolutionParams.size() > 0);
        auto& card = ecs.getCard(effect.sourceEntityId);
        moveCard(effect.sourceEntityId, static_cast<CardLocation>(effect.resolutionParams[0]));
    }
    
}
