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
        CardLocation pileAfterPlay = CardLocation::Discard;
        int energyCost = 0, starCost = 0;
        int damage = 0, block = 0, replay = 0;
        int vulnerable = 0, weak = 0;
        int cardAnyParam1 = 0, cardAnyParam2 = 0, cardAnyParam3 = 0;
        
        void addEffect(EffectPool &effectPool, EffectId effectId) {
            effects.push_back(effectPool.retrieve(effectId));
        }
        
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CardData,
            effects, playableType, targetingType, pileAfterPlay,
            energyCost, starCost,
            damage, block, replay,
            vulnerable, weak,
            cardAnyParam1, cardAnyParam2, cardAnyParam3
        );
        TO_STRING_METHOD
    
        // std::string toString() {
        //     std::stringstream ss;
        //     ss << "energyCost: " << energyCost << "; starCost: " << starCost;
        //     ss << "; pileAfterPlay: " << ToString(pileAfterPlay) << "; targetingType: " << int(targetingType);
        //     ss << "; damage: " << damage << "; block: " << block << "; replay: " << replay;
        //     ss << "; Effects: { ";
        //     for(auto effect : effects) ss << "[ " << effect.toString() << "] ";
        //     ss << "} ";
        //     return ss.str();
        // }
    };

}
