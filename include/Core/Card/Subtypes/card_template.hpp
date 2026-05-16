#pragma once

#include <Core/Event/event_listener.hpp>
#include <Core/Card/Subtypes/card_id.hpp>
#include <Core/Card/Subtypes/card_data.hpp>


namespace SpireSim {

    struct CardTemplate {
        CardId cardId;
        CardData normalData, upgradedData;
        EventList eventList;
        
        CardTemplate() {}
        
        void derive(Card &card) {
            card.cardId = cardId;
            if(card.isUpgraded) {
                card.data = upgradedData;
            } else {
                card.data = normalData;
            }
        }
        
        void gainEffect(EffectPool &effectPool, EffectId effectId) {
            normalData.gainEffect(effectPool, effectId);
            upgradedData.gainEffect(effectPool, effectId);
        }

        void applyVulnerable(EffectPool &effectPool, int value, int valueUpgraded) {
            normalData.vulnerable = value;
            upgradedData.vulnerable = valueUpgraded;
            gainEffect(effectPool, EffectId::CardApplyVulnerable);
        }

        void moveCardsFromDiscardToDeck(EffectPool &effectPool, int value, int valueUpgraded) {
            normalData.cardsToChoose = value;
            upgradedData.cardsToChoose = valueUpgraded;
            gainEffect(effectPool, EffectId::ChooseCardsFromDiscard);
            gainEffect(effectPool, EffectId::MoveChosenCardsToDeck);
        }
        
        void returnCardsToHand(EffectPool &effectPool, int value, int valueUpgraded) {
            normalData.cardsToChoose = value;
            upgradedData.cardsToChoose = valueUpgraded;
            gainEffect(effectPool, EffectId::ChooseCardsFromDiscard);
            gainEffect(effectPool, EffectId::MoveChosenCardsToHand);
        }
        
        void copyUpgradedDataFromNormal() {
            upgradedData = normalData;
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "cardId: " << ToString(cardId) << "; normal: [" << normalData.toString()
                << " ]; upgraded: [" << upgradedData.toString() << "]";
            return ss.str();
        }
    };
    using CardTemplates = std::vector<CardTemplate>;

}
