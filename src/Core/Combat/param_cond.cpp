#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    inline int Combat::resolveParam(Param &param, Id sourceEntityId) {
        switch(param.paramType) {
            case ParamType::FixedValue:             return param.value;
            case ParamType::CardAttackDamage:       return ecs.getCard(sourceEntityId).data.damage;
            case ParamType::CardBlockValue:         return ecs.getCard(sourceEntityId).data.block;
            case ParamType::CardApplyVulnerable:    return ecs.getCard(sourceEntityId).data.vulnerable;
            case ParamType::CardsPlayedThisCombat:  return variables.cardsPlayedThisCombat;
            case ParamType::XValue:                 return variables.xValue;
            default: assert(false);
        }
    }
    
    inline void Combat::resolveParams(Effect &effect) {
        effect.resolutionParams.clear();
        for(auto& param : effect.params)
            effect.resolutionParams.push_back(resolveParam(param, effect.sourceEntityId));
    }
    
    inline bool Combat::evaluateCondition(Condition &condition, Id sourceEntityId) {
        auto value1 = resolveParam(condition.param1, sourceEntityId);
        auto value2 = resolveParam(condition.param2, sourceEntityId);
        return condition.evaluate(value1, value2);
    }
    
    inline bool Combat::evaluateConditions(Effect &effect) {
        for(auto& condition : effect.conditions) {
            if(!evaluateCondition(condition, effect.sourceEntityId)) return false;
        }
        return true;
    }
    
}
