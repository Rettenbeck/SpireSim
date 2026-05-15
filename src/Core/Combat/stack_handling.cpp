#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::putEffectOntoStack(const Effect &effect, int position) {
        stack.insert(stack.begin() + position, effect);
    }

    void CombatState::putCardOntoStack(Id cardEntityId, int targetEntityId) {
        putEffectOntoStack(Effect(EffectType::UnpackCard, {}, cardEntityId));
    }
    
    void CombatState::triggerEvent(EventType eventType) {
        auto& listeners = eventRegistry[int(eventType)];
        for(auto& listener : listeners) {
            if(listener.inactive) continue;
            auto id = listener.entityId;
            if(!evaluateConditions(listener.effect)) continue;
            auto& e = listener.effect;
            putEffectOntoStack(Effect(e.effectType, e.params, e.sourceEntityId));
        }
    }

    void CombatState::resolve(Effect &effect) {
        if(!evaluateConditions(effect)) return;
        resolveParams(effect);
        switch(effect.effectType) {
            case EffectType::UnpackCard: executeUnpackCard(effect); return;
            case EffectType::FinishCardPlayed: executeFinishCardPlayed(effect); return;
            case EffectType::CardDealDamage: executeCardDealDamage(effect); return;
            case EffectType::CardGainBlock : executeCardGainBlock (effect); return;
            case EffectType::CardApplyVulnerable: executeCardApplyVulnerable(effect); return;
            case EffectType::MoveCard: executeMoveCard(effect); return;
            default: assert(false);
        }
    }
    
}
