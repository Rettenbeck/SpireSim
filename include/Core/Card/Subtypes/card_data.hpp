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
        bool retain = false;
        int energyCost = 0, starCost = 0;
        int damage = 0, block = 0, replay = 0;
        int vulnerable = 0, weak = 0;
        int cardAnyParam1 = 0, cardAnyParam2 = 0, cardAnyParam3 = 0;

        // Param1 -> Draw X cards; choose X cards; return X cards
        // Param2 -> Gain X energy; gain X stars, gain X vigor
        // Param3 -> Lose X life; increase claw damage by X; create Card with id X
        
        void addEffect(EffectPool &effectPool, EffectId effectId) {
            effects.push_back(effectPool.retrieve(effectId));
        }
        
        void addEffectInFront(EffectPool &effectPool, EffectId effectId) {
            effects.insert(effects.begin(), effectPool.retrieve(effectId));
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
