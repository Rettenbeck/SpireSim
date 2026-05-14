#pragma once

#include <Core/Card/Effect/effect_pool.hpp>
#include <Core/Card/Subtypes/card_location.hpp>
#include <Core/Card/Subtypes/playable_type.hpp>
#include <Core/Card/Subtypes/targeting_type.hpp>


namespace SpireSim {

    const int ENERGY_COST_NONE = -1;

    struct CardData {
        Effects effects;
        PlayableType playableType = PlayableType::Playable;
        TargetingType targetingType = TargetingType::None;
        int energyCost = 0, starCost = 0;
        CardLocation pileAfterPlay = CardLocation::Discard;
        int damage = 0, block = 0, replay = 0;
        int vulnerable = 0, weak = 0, fragile = 0;
        
        void gainEffect(EffectPool &effectPool, EffectId effectId) {
            effects.push_back(effectPool.retrieve(effectId));
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "energyCost: " << energyCost << "; starCost: " << starCost;
            ss << "; pileAfterPlay: " << ToString(pileAfterPlay) << "; targetingType: " << int(targetingType);
            ss << "; damage: " << damage << "; block: " << block << "; replay: " << replay;
            ss << "; Effects: { ";
            for(auto effect : effects) ss << "[ " << effect.toString() << "] ";
            ss << "} ";
            return ss.str();
        }
    };

}
