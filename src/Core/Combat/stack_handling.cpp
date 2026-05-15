#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::putEffectOntoStack(const Effect &effect, int position) {
        stack.insert(stack.begin() + position, effect);
        resolveStackFully();
    }

    void CombatState::putCardOntoStack(Id cardEntityId, int targetEntityId) {
        putEffectOntoStack(Effect(EffectType::UnpackCard, {}, cardEntityId));
    }
    
    void CombatState::resolveInterceptor(Effect &effect, const InterceptorContext &context, int &value) {
        if(!evaluateConditions(effect)) return;
        resolveParams(effect);
        switch(effect.effectType) {
            case EffectType::ModifyParentDamagePerc:    executeModifyParentDamagePerc(effect, context, value); return;
            default: assert(false);
        }
    }

    void CombatState::resolve(Effect &effect) {
        if(!evaluateConditions(effect)) return;
        resolveParams(effect);
        switch(effect.effectType) {
            case EffectType::UnpackCard:            executeUnpackCard           (effect); return;
            case EffectType::FinishCardPlayed:      executeFinishCardPlayed     (effect); return;
            case EffectType::DrawCards:             executeDrawCards            (effect); return;
            case EffectType::CardDealDamage:        executeCardDealDamage       (effect); return;
            case EffectType::CardGainBlock:         executeCardGainBlock        (effect); return;
            case EffectType::CardApplyVulnerable:   executeCardApplyVulnerable  (effect); return;
            case EffectType::MoveCard:              executeMoveCard             (effect); return;
            default: assert(false);
        }
    }
    
    void CombatState::resolveStackSingle() {
        if(stack.size() == 0) return;
        auto effect = stack[0];
        stack.erase(stack.begin());
        resolve(effect);
    }
    
    void CombatState::resolveStackFully() {
        if(isStackRunning) return;
        isStackRunning = true;
        while(stack.size() > 0 && waitingForActionOnStack == false && stopStack == false) {
            resolveStackSingle();
        }
        isStackRunning = false;
    }

}
