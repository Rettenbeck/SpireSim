#pragma once

#include <Core/Card/Effect/effect_id.hpp>
#include <Core/Card/Effect/effect.hpp>


namespace SpireSim {

    struct EffectPool {
        Effects effects;
        
        Effect& retrieveForCreation(EffectId effectId) {
            int index = int(effectId);
            while(index >= effects.size()) {
                effects.push_back(Effect());
            }
            return effects[index];
        }
        
        Effect& retrieve(EffectId effectId) {
            int index = int(effectId);
            assert(index < effects.size());
            return effects[index];
        }
        
        void createEffects() {
            effects.clear();
            
            retrieveForCreation(EffectId::CardDealDamage)
            .fill(EffectType::CardDealDamage, {Param(ParamType::CardAttackDamage, 0)}, {});
            
            retrieveForCreation(EffectId::CardGainBlock)
            .fill(EffectType::CardGainBlock, {Param(ParamType::CardBlockValue, 0)}, {});
            
            retrieveForCreation(EffectId::CardApplyVulnerable)
            .fill(EffectType::CardApplyVulnerable, {Param(ParamType::CardApplyVulnerable, 0)}, {});
        }
        
        EffectPool() {
            createEffects();
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "Effect pool: { ";
            for(auto effect : effects) ss << "[ " << effect.toString() << "] ";
            ss << "}";
            return ss.str();
        }
    };

}
