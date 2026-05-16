#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    void Combat::putEffectOntoStack(const Effect &effect, int position) {
        state.stack.insert(state.stack.begin() + position, effect);
        resolveStackFully();
    }

    void Combat::putCardOntoStack(Id cardEntityId, int targetEntityId) {
        putEffectOntoStack(Effect(EffectType::UnpackCard, {}, cardEntityId));
    }
    
    void Combat::resolveInterceptor(Effect &effect, const InterceptorContext &context, int &value) {
        if(!evaluateConditions(effect)) return;
        resolveParams(effect);
        switch(effect.effectType) {
            case EffectType::ModifyParentDamagePerc:    executeModifyParentDamagePerc(effect, context, value); return;
            default: assert(false);
        }
    }

    void Combat::resolve(Effect &effect) {
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
    
    void Combat::resolveStackSingle() {
        if(state.stack.size() == 0) return;
        auto effect = state.stack[0];
        state.stack.erase(state.stack.begin());
        resolve(effect);
    }
    
    void Combat::resolveStackFully() {
        if(state.isStackRunning) return;
        state.isStackRunning = true;
        while(state.stack.size() > 0 && state.waitingForActionOnStack == false && state.stopStack == false) {
            resolveStackSingle();
        }
        state.isStackRunning = false;
    }

}
