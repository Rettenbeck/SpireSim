#pragma once

#include <Core/Card/Modifiers/enchantment.hpp>
#include <Core/Card/Modifiers/modification.hpp>
#include <Core/Card/Subtypes/card_id.hpp>
#include <Core/Card/Subtypes/card_data.hpp>


namespace SpireSim {

    struct Card {
        CardId cardId = CardId::None;
        CardLocation location = CardLocation::None;
        int locationIndex;
        bool isUpgraded;
        Id targetEntityId = ENTITY_NONE;
        CardData data;
        Enchantment enchantment;
        Modifications modifications;
        
        Card() {
            enchantment.enchantmentType = EnchantmentType::None;
        }
        
        Card(CardId cardId_) : cardId(cardId_) {
            isUpgraded = false;
            enchantment.enchantmentType = EnchantmentType::None;
        }
        
        Card(CardId cardId_, bool isUpgraded_) : cardId(cardId_), isUpgraded(isUpgraded_) {
            enchantment.enchantmentType = EnchantmentType::None;
        }
        
        Card(CardId cardId_, const Enchantment &enchantment_) : cardId(cardId_), enchantment(enchantment_) {
            isUpgraded = false;
        }
        
        Card(CardId cardId_, bool isUpgraded_, Enchantment &enchantment_) : cardId(cardId_),
            isUpgraded(isUpgraded_), enchantment(enchantment_) {}
        
        void addModification(Modification &modification) {
            modifications.push_back(modification);
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "cardId: " << ToString(cardId) << "; Location: " << ToString(location) << "; locationIndex: " << locationIndex << "; isUpgraded: " << isUpgraded;
            ss << "; Attributes: [ " << data.toString() << " ]";
            ss << "; Enchantment: " << enchantment.toString();
            if(modifications.size() > 0) {
                ss << "; Modifications: { ";
                for(auto modification : modifications) {
                    ss << modification.toString() << "; ";
                }
            ss << "}";
            }
            return ss.str();
        }
    };
    using Cards = std::vector<Card>;

}
