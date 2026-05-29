#pragma once

#include <Core/Card/card.hpp>
#include <Core/Card/Subtypes/card_template.hpp>
#include <Core/Card/card_data_base.hpp>


namespace SpireSim {

    struct CardPool {
        CardTemplates cardTemplates;
        EffectPool& effectPool;
        CardDataBase cardDataBase;
        
        CardPool(EffectPool &effectPool_) : effectPool(effectPool_) {
            createCardPool();
            cardDataBase.check();
        }
        
        void createBlankArray(int index) {
            while(index >= cardTemplates.size()) {
                cardTemplates.push_back(CardTemplate(effectPool));
            }
        }
        
        CardTemplate& retrieveForCreation(CardId cardId) {
            int index = int(cardId);
            createBlankArray(index);
            assert(cardTemplates.size() > index);
            assert(cardTemplates.size() < 9999);
            auto& cardTemplate = cardTemplates[index];
            cardTemplate.cardId = cardId;
            auto dbEntry = cardDataBase.find(cardId);
            assert(dbEntry);
            cardTemplate.cardType = dbEntry->cardType;
            return cardTemplate;
        }
        
        CardTemplate& retrieveForCreation(CardId cardId, int energyCostNormal, int energyCostUpgraded) {
            auto& cardTemplate = retrieveForCreation(cardId);
            cardTemplate.normalData.energyCost = energyCostNormal;
            cardTemplate.upgradedData.energyCost = energyCostUpgraded;
            return cardTemplate;
        }
        
        CardTemplate& retrieveForCreation(CardId cardId, int energyCost) {
            return retrieveForCreation(cardId, energyCost, energyCost);
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


            // Basics
            retrieveForCreationSingleAttack(CardId::Strike, 1, 6, 9);
            retrieveForCreationBlock(CardId::Defend, 1, 5, 8);


            // Ironclad
            retrieveForCreationSingleAttack(CardId::Bash, 2, 8, 11).applyVulnerable(2, 3);
            retrieveForCreationAOEAttack(CardId::Thunderclap, 1, 4, 7).applyVulnerable(1, 1);
            retrieveForCreation(CardId::BloodLetting, 0).loseLifeSelf(3).gainEnergy(2, 3);


            // Silent
            retrieveForCreationSingleAttack(CardId::Neutralize, 0, 3, 4).applyWeak(1, 2);


            // Regent
            retrieveForCreation(CardId::Venerate, 1).gainStars(2, 3);
            retrieveForCreationSingleAttack(CardId::FallingStar, 0, 8, 12)
                .addStarCost(2).applyWeak(1).applyVulnerable(1);
            
            retrieveForCreationAOEAttack(CardId::AstralPulse, 0, 14, 18).addStarCost(3);
            retrieveForCreationSingleAttack(CardId::CelestialMight, 2, 6, 6).copyLastAddedEffect(2, 3);

            retrieveForCreationBlock(CardId::CosmicIndifference, 1, 6, 9)
                .chooseCards(1, 2, CardLocation::Discard)
                .moveChosenCardsToTarget(CardLocation::Deck);
            
            retrieveForCreationSingleAttack(CardId::MakeItSo, 0, 6, 9).returnToHandAfterXCards(3);

            // Defect
            retrieveForCreationBlock(CardId::Hologram, 1, 3, 5)
                .chooseCards(1, 1, CardLocation::Discard)
                .moveChosenCardsToTarget(CardLocation::Hand);
            
            retrieveForCreationSingleAttack(CardId::Claw, 0, 3, 4)
                .addEffectInFront(EffectId::CardModifyDamageFlatClaw)
                .sharpenClaws(2, 3);
            
            
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
