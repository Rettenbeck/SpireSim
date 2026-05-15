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
