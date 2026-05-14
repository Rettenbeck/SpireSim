#pragma once

#include <Core/Card/card.hpp>
#include <Core/Card/Subtypes/card_template.hpp>


namespace SpireSim {

    struct CardPool {
        CardTemplates cardTemplates;
        
        CardPool(EffectPool &effectPool) {
            createCardPool(effectPool);
        }
        
        void createBlankArray(int index) {
            while(index >= cardTemplates.size()) {
                cardTemplates.push_back(CardTemplate());
            }
        }
        
        CardTemplate& retrieveForCreation(CardId cardId) {
            int index = int(cardId);
            createBlankArray(index);
            cardTemplates[index].cardId = cardId;
            return cardTemplates[index];
        }
        
        CardTemplate& retrieve(CardId cardId) {
            int index = int(cardId);
            assert(index < cardTemplates.size());
            return cardTemplates[index];
        }
        
        CardData& retrieveData(CardId cardId, bool isUpgraded = false) {
            auto& cardTemplate = retrieve(cardId);
            switch(isUpgraded) {
                case false: return cardTemplate.normalData;
                case true : return cardTemplate.upgradedData;
            }
        }
        
        CardData& retrieveData(Card &card) {
            return retrieveData(card.cardId, card.isUpgraded);
        }
        
        Card createCardFromTemplate(CardId cardId) {
            Card card;
            retrieveForCreation(cardId).derive(card);
            return card;
        }
        
        Card createCardFromTemplate(CardId cardId, bool isUpgraded) {
            Card card;
            card.isUpgraded = isUpgraded;
            retrieveForCreation(cardId).derive(card);
            return card;
        }
        
        Card createCardFromTemplateLazy(CardId cardId) {
            return Card(cardId);
        }
        
        void derive(Card &card, CardId cardId) {
            retrieve(cardId).derive(card);
        }
        
        void applyModifiers(Card &card) {
            derive(card, card.cardId);
            // apply modifiers...
            std::cout << "applyModifiers: " << int(card.cardId) << "\n";
        }
        
        void createCardPool(EffectPool &effectPool) {
            createBlankArray(int(CardId::Count));
            auto& cardStrike = retrieveForCreation(CardId::Strike);
            auto& cardDefend = retrieveForCreation(CardId::Defend);
            auto& cardBash = retrieveForCreation(CardId::Bash);
            auto& cardThunderclap = retrieveForCreation(CardId::Thunderclap);
            auto& cardMakeItSo = retrieveForCreation(CardId::MakeItSo);
            
            cardStrike.normalData.targetingType = TargetingType::Single;
            cardStrike.normalData.energyCost = 1;
            cardStrike.normalData.damage = 6;
            cardStrike.normalData.gainEffect(effectPool, EffectId::CardDealDamage);
            cardStrike.copyUpgradedDataFromNormal();
            cardStrike.upgradedData.damage = 9;
            
            cardDefend.normalData.targetingType = TargetingType::None;
            cardDefend.normalData.energyCost = 1;
            cardDefend.normalData.block = 5;
            cardDefend.normalData.gainEffect(effectPool, EffectId::CardGainBlock);
            cardDefend.copyUpgradedDataFromNormal();
            cardDefend.upgradedData.block = 8;
            
            cardBash.normalData.targetingType = TargetingType::Single;
            cardBash.normalData.energyCost = 2;
            cardBash.normalData.damage = 8;
            cardBash.normalData.vulnerable = 2;
            cardBash.normalData.gainEffect(effectPool, EffectId::CardDealDamage);
            cardBash.normalData.gainEffect(effectPool, EffectId::CardApplyVulnerable);
            cardBash.copyUpgradedDataFromNormal();
            cardBash.upgradedData.damage = 11;
            cardBash.upgradedData.vulnerable = 3;
            
            cardThunderclap.normalData.targetingType = TargetingType::All;
            cardThunderclap.normalData.energyCost = 1;
            cardThunderclap.normalData.damage = 4;
            cardThunderclap.normalData.vulnerable = 1;
            cardThunderclap.normalData.gainEffect(effectPool, EffectId::CardDealDamage);
            cardThunderclap.normalData.gainEffect(effectPool, EffectId::CardApplyVulnerable);
            cardThunderclap.copyUpgradedDataFromNormal();
            cardThunderclap.upgradedData.damage = 7;
            
            cardMakeItSo.normalData.targetingType = TargetingType::Single;
            cardMakeItSo.normalData.energyCost = 0;
            cardMakeItSo.normalData.damage = 6;
            cardMakeItSo.normalData.gainEffect(effectPool, EffectId::CardDealDamage);
            cardMakeItSo.copyUpgradedDataFromNormal();
            cardMakeItSo.upgradedData.damage = 9;
            cardMakeItSo.eventList[int(EventType::OnCardPlayed)].push_back(EventListener(
                Effect( EffectType::MoveCard,
                        {Param(ParamType::FixedValue, int(CardLocation::Hand))},
                        {Condition( ConditionType::DivisibleBy,
                                    Param(ParamType::CardsPlayedThisCombat),
                                    Param(ParamType::FixedValue, 3))}
                        )));
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "CardPool: { ";
            for(auto cardTemplate : cardTemplates) ss << "[ " << cardTemplate.toString() << "] ";
            ss << "};";
            return ss.str();
        }
    };

}
