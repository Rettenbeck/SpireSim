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
                cardTemplates.push_back(CardTemplate(effectPool));
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
            card.addEffect(EffectId::CardDealDamage);
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
            card.addEffect(EffectId::CardGainBlock);
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
        }
        
        void createCardPool() {
            createBlankArray(int(CardId::Count));

            retrieveForCreationSingleAttack(CardId::Strike, 1, 6, 9);
            retrieveForCreationBlock(CardId::Defend, 1, 5, 8);
            retrieveForCreationSingleAttack(CardId::Bash, 2, 8, 11).applyVulnerable(2, 3);
            retrieveForCreationAOEAttack(CardId::Thunderclap, 1, 4, 7).applyVulnerable(1, 1);
            retrieveForCreationSingleAttack(CardId::MakeItSo, 0, 6, 9).returnToHandAfterXCards(3);
            
            retrieveForCreationBlock(CardId::CosmicIndifference, 1, 6, 9)
                .chooseCards(1, 2, CardLocation::Discard)
                .moveChosenCardsToTarget(CardLocation::Deck);
            
            retrieveForCreationBlock(CardId::Hologram, 1, 3, 5)
                .chooseCards(1, 1, CardLocation::Discard)
                .moveChosenCardsToTarget(CardLocation::Hand);
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
