#pragma once

#include <Core/Event/event_listener.hpp>
#include <Core/Card/Subtypes/card_id.hpp>
#include <Core/Card/Subtypes/card_data.hpp>


namespace SpireSim {

    struct CardTemplate {
        EffectPool& effectPool;
        CardId cardId;
        CardData normalData, upgradedData;
        EventList eventList;
        
        CardTemplate(EffectPool &effectPool_) : effectPool(effectPool_) {}
        
        void derive(Card &card) {
            card.cardId = cardId;
            if(card.isUpgraded) {
                card.data = upgradedData;
            } else {
                card.data = normalData;
            }
        }
        
        void copyUpgradedDataFromNormal() {
            upgradedData = normalData;
        }
        
        CardTemplate& addEffect(EffectId effectId) {
            normalData.addEffect(effectPool, effectId);
            upgradedData.addEffect(effectPool, effectId);
            return *this;
        }

        CardTemplate& addEvent(const EventListEntry &event) {
            eventList.push_back(event);
            return *this;
        }

        CardTemplate& addEvent(EventType eventType, const EventListener eventListener) {
            return addEvent({eventType, eventListener});
        }

        CardTemplate& applyVulnerable(int value, int valueUpgraded) {
            normalData.vulnerable = value;
            upgradedData.vulnerable = valueUpgraded;
            addEffect(EffectId::CardApplyVulnerable);
            return *this;
        }

        CardTemplate& chooseCards(int value, int valueUpgraded, CardLocation cardLocation) {
            normalData.cardsToChoose = value;
            upgradedData.cardsToChoose = valueUpgraded;
            EffectId effectId;
            switch (cardLocation) {
                case CardLocation::Deck   : effectId = EffectId::ChooseCardsFromDeck   ; break;
                case CardLocation::Discard: effectId = EffectId::ChooseCardsFromDiscard; break;
                default: assert(false);
            }
            addEffect(effectId);
            return *this;
        }
        
        CardTemplate& moveChosenCardsToTarget(CardLocation cardLocation) {
            EffectId effectId;
            switch (cardLocation) {
                case CardLocation::Deck: effectId = EffectId::MoveChosenCardsToDeck; break;
                case CardLocation::Hand: effectId = EffectId::MoveChosenCardsToHand; break;
                default: assert(false);
            }
            addEffect(effectId);
            return *this;
        }
        
        CardTemplate& returnCardsToHand(int value, int valueUpgraded) {
            normalData.cardsToChoose = value;
            upgradedData.cardsToChoose = valueUpgraded;
            addEffect(EffectId::ChooseCardsFromDiscard);
            addEffect(EffectId::MoveChosenCardsToHand);
            return *this;
        }
        
        CardTemplate& returnToHandAfterXCards(int value) {
            return addEvent(EventType::OnCardPlayed, EventListener(
                    Effect( EffectType::MoveCard,
                            {Param(ParamType::FixedValue, int(CardLocation::Hand))},
                            {Condition( ConditionType::DivisibleBy,
                                        Param(ParamType::CardsPlayedThisCombat),
                                        Param(ParamType::FixedValue, value))}
                            )));
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
