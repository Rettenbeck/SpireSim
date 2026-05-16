#pragma once

#include <Core/Card/card.hpp>
#include <Core/Card/Subtypes/card_template.hpp>


namespace SpireSim {

    struct CardPool {
        CardTemplates cardTemplates;
        EffectPool& effectPool;
        
        CardPool(EffectPool &effectPool_) : effectPool(effectPool_) {
            createCardPool();
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
        
        CardTemplate& retrieveForCreationAttack(CardId cardId,
                                                TargetingType targetingType,
                                                int energyCost,
                                                int damage,
                                                int damageUpgraded)
        {
            auto& card = retrieveForCreation(cardId);
            card.normalData.targetingType = targetingType;
            card.normalData.energyCost = energyCost;
            card.normalData.damage = damage;
            card.normalData.gainEffect(effectPool, EffectId::CardDealDamage);
            card.copyUpgradedDataFromNormal();
            card.upgradedData.damage = damageUpgraded;
            return card;
        }
        
        CardTemplate& retrieveForCreationBlock( CardId cardId,
                                                int energyCost,
                                                int block,
                                                int blockUpgraded)
        {
            auto& card = retrieveForCreation(cardId);
            card.normalData.targetingType = TargetingType::None;
            card.normalData.energyCost = energyCost;
            card.normalData.block = block;
            card.normalData.gainEffect(effectPool, EffectId::CardGainBlock);
            card.copyUpgradedDataFromNormal();
            card.upgradedData.block = blockUpgraded;
            return card;
        }
        
        CardTemplate& retrieveForCreationSingleAttack(CardId cardId, int energyCost, int damage, int damageUpgraded) {
            return retrieveForCreationAttack(cardId, TargetingType::Single, energyCost, damage, damageUpgraded);
        }
        
        CardTemplate& retrieveForCreationAOEAttack(CardId cardId, int energyCost, int damage, int damageUpgraded) {
            return retrieveForCreationAttack(cardId, TargetingType::All, energyCost, damage, damageUpgraded);
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
        
        void createCardPool() {
            createBlankArray(int(CardId::Count));
            auto& cardStrike = retrieveForCreationSingleAttack(CardId::Strike, 1, 6, 9);
            auto& cardDefend = retrieveForCreationBlock(CardId::Defend, 1, 5, 8);
            auto& cardBash = retrieveForCreationSingleAttack(CardId::Bash, 2, 8, 11);
            auto& cardThunderclap = retrieveForCreationAOEAttack(CardId::Thunderclap, 1, 4, 7);
            auto& cardMakeItSo = retrieveForCreationSingleAttack(CardId::MakeItSo, 0, 6, 9);
            auto& cardCosmicIndifference = retrieveForCreationBlock(CardId::CosmicIndifference, 1, 6, 9);
            
            cardBash.normalData.vulnerable = 2;
            cardBash.upgradedData.vulnerable = 3;
            cardBash.gainEffect(effectPool, EffectId::CardApplyVulnerable);
            
            cardThunderclap.normalData.vulnerable = 1;
            cardThunderclap.upgradedData.vulnerable = 1;
            cardThunderclap.gainEffect(effectPool, EffectId::CardApplyVulnerable);
            
            cardMakeItSo.eventList.push_back({EventType::OnCardPlayed, EventListener(
                Effect( EffectType::MoveCard,
                        {Param(ParamType::FixedValue, int(CardLocation::Hand))},
                        {Condition( ConditionType::DivisibleBy,
                                    Param(ParamType::CardsPlayedThisCombat),
                                    Param(ParamType::FixedValue, 3))}
                        ))});
            
            cardCosmicIndifference.gainEffect(effectPool, EffectId::ChooseCardsFromDiscard);
            cardCosmicIndifference.gainEffect(effectPool, EffectId::MoveChosenCardsToDeck);
            cardCosmicIndifference.normalData.cardsToChoose = 1;
            cardCosmicIndifference.upgradedData.cardsToChoose = 2;
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
