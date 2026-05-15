#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::executeModifyParentDamagePerc(Effect &effect, InterceptorContext &context, int &value) {
        // effect.sourceEntityId        -> Source of effect (typically the entity Id of a buff)
        // effect.sourceEntityId parent -> Entity Id that owns the effect source (typically the character owning the buff)
        // context.sourceEntityId       -> Source of interceptor event

        // Only if the buff's owner is also source of the event (that is about to deal damage) we actually do something
        if(ecs.getParent(effect.sourceEntityId) != context.sourceEntityId) return;

        assert(effect.resolutionParams.size() > 0);
        int& percentage = effect.resolutionParams[0];

        value = ((float) value) * (((float) percentage) / 100.0f);
    }
    
}
