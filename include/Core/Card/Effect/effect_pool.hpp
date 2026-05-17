#pragma once

#include <Core/Card/Effect/effect_id.hpp>
#include <Core/Card/Effect/effect.hpp>
#include <Core/Card/Subtypes/card_location.hpp>


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
            .fill(EffectType::CardDealDamage, {Param(ParamType::CardAttackDamage)}, {});
            
            retrieveForCreation(EffectId::CardGainBlock)
            .fill(EffectType::CardGainBlock, {Param(ParamType::CardBlockValue)}, {});
            
            retrieveForCreation(EffectId::CardApplyVulnerable)
            .fill(EffectType::CardApplyVulnerable, {Param(ParamType::CardApplyVulnerable)}, {});
            
            retrieveForCreation(EffectId::CardApplyWeak)
            .fill(EffectType::CardApplyWeak, {Param(ParamType::CardApplyWeak)}, {});
            
            retrieveForCreation(EffectId::ChooseCardsFromDeck)
            .fill(EffectType::ChooseCards,
                    {   Param(ParamType::CardAnyParam1),
                        Param(ParamType::FixedValue, int(CardLocation::Deck))
                    }, {});
            
            retrieveForCreation(EffectId::ChooseCardsFromDiscard)
            .fill(EffectType::ChooseCards,
                    {   Param(ParamType::CardAnyParam1),
                        Param(ParamType::FixedValue, int(CardLocation::Discard))
                    }, {});

            retrieveForCreation(EffectId::MoveChosenCardsToDeck)
            .fill(EffectType::MoveChosenCards, {Param(ParamType::FixedValue, int(CardLocation::Deck))}, {});

            retrieveForCreation(EffectId::MoveChosenCardsToHand)
            .fill(EffectType::MoveChosenCards, {Param(ParamType::FixedValue, int(CardLocation::Hand))}, {});
            
            retrieveForCreation(EffectId::CardDrawCards)
            .fill(EffectType::DrawCards, {Param(ParamType::CardAnyParam1)}, {});
            
            retrieveForCreation(EffectId::CardGainEnergy)
            .fill(EffectType::GainEnergy, {Param(ParamType::CardAnyParam2)}, {});
            
            retrieveForCreation(EffectId::CardGainStars)
            .fill(EffectType::GainStars, {Param(ParamType::CardAnyParam2)}, {});
            
            retrieveForCreation(EffectId::CardLoseLifeSelf)
            .fill(EffectType::LoseLifeSelf, {Param(ParamType::CardAnyParam3)}, {});
            
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
