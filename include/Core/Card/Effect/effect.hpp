#pragma once

#include <Core/Combat/entity.hpp>
#include <Core/Card/Effect/condition.hpp>
#include <Core/Card/Effect/effect_type.hpp>


namespace SpireSim {

    using ResolutionParams = std::vector<int>;

    struct Effect {
        EffectType effectType;
        Params params;
        Conditions conditions;
        ResolutionParams resolutionParams;
        Id sourceEntityId = ENTITY_NONE;
        // Id affectedEntityId = ENTITY_NONE;
        
        void addParams(const Params &params_) {
            for(auto param : params_) params.push_back(param);
        }
        
        void addConditions(const Conditions &conditions_) {
            for(auto condition : conditions_) conditions.push_back(condition);
        }
        
        Effect() { effectType = EffectType::None; }
        Effect(EffectType effectType_) : effectType(effectType_) {}
        Effect(EffectType effectType_, const Params &params_) : effectType(effectType_) {
            addParams(params_);
        }
        Effect(EffectType effectType_, const Params &params_, Id sourceEntityId_) : effectType(effectType_), sourceEntityId(sourceEntityId_) {
            addParams(params_);
        }
        Effect(EffectType effectType_, const Conditions conditions_) : effectType(effectType_) {
            addConditions(conditions_);
        }
        Effect(EffectType effectType_, const Params &params_, const Conditions &conditions_) : effectType(effectType_) {
            addParams(params_);
            addConditions(conditions_);
        }
        
        void fill(EffectType effectType_, Params params_, Conditions conditions_) {
            effectType = effectType_;
            addParams(params_);
            addConditions(conditions_);
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "effectType: " << ToString(effectType)
                << "; sourceEntityId: " << sourceEntityId
                // << "; affectedEntityId: " << affectedEntityId
                << "; ";
            if(params.size() > 0) {
                ss << "; Params: { ";
                for(auto param : params) {
                    ss << param.toString() << "; ";
                }
                ss << "} ";
            }
            if(params.size() > 0) {
                ss << "; Conditions: { ";
                for(auto condition : conditions) {
                    ss << condition.toString() << "; ";
                }
                ss << "} ";
            }
            return ss.str();
        }
    };
    using Effects = std::vector<Effect>;

}
