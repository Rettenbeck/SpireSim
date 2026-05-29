#pragma once

#include <Core/Card/Subtypes/card_id.hpp>
#include <Core/Card/Subtypes/card_type.hpp>
#include <Core/Card/Subtypes/card_rarity.hpp>
#include <Core/Character/player_id.hpp>


namespace SpireSim {

    #define ADDC(NAME,TYPE,RARITY,PLAYER) \
        entries.push_back(CardDataBaseEntry(CardId::NAME, CardType::TYPE, CardRarity::RARITY, PlayerId::PLAYER, #NAME));

    #define ADDF(NAME,TYPE,RARITY,PLAYER,IS_STRIKE,IS_DEFEND) \
        entries.push_back(CardDataBaseEntry(CardId::NAME, CardType::TYPE, CardRarity::RARITY, PlayerId::PLAYER, #NAME, IS_STRIKE, IS_DEFEND));

    struct CardDataBaseEntry {
        CardId cardId = CardId::None;
        CardType cardType = CardType::None;
        CardRarity rarity = CardRarity::None;
        PlayerId playerId = PlayerId::None;
        std::string name = "NONE";
        bool isStrike = false;
        bool isDefend = false;

        CardDataBaseEntry() {}

        CardDataBaseEntry(CardId cardId_, CardType cardType_, CardRarity rarity_,
            PlayerId playerId_, std::string name_) :
                cardId(cardId_), cardType(cardType_), rarity(rarity_), playerId(playerId_), name(name_) {}
        
        CardDataBaseEntry(CardId cardId_, CardType cardType_, CardRarity rarity_,
            PlayerId playerId_, std::string name_, bool isStrike_, bool isDefend_) :
                cardId(cardId_), cardType(cardType_), rarity(rarity_), playerId(playerId_), name(name_),
                isStrike(isStrike_), isDefend(isDefend_) {}
    };
    using CardDataBaseEntries = std::vector<CardDataBaseEntry>;

    struct CardDataBase {
        CardDataBaseEntries entries;

        CardDataBase() {
            fill();
        }

        void fill() {
            entries.clear();

            // Basics
            ADDF(Strike                     , Attack, Basic     , Colorless , true  , false )
            ADDF(Defend                     , Skill , Basic     , Colorless , false , true  )

            // Ironclad
            ADDC(Bash                       , Attack, Basic     , IronClad  )
            ADDC(Thunderclap                , Attack, Common    , IronClad  )
            ADDC(BloodLetting               , Skill , Common    , IronClad  )

            // Silent
            ADDC(Neutralize                 , Attack, Basic     , Silent    )

            // Regent
            ADDC(Venerate                   , Skill , Basic     , Regent    )
            ADDC(FallingStar                , Attack, Basic     , Regent    )
            ADDC(AstralPulse                , Attack, Common    , Regent    )
            ADDC(CosmicIndifference         , Skill , Common    , Regent    )
            ADDC(MakeItSo                   , Attack, Rare      , Regent    )

            // Defect
            ADDC(Hologram                   , Skill , Common    , Defect    )
            ADDC(Claw                       , Attack, Common    , Defect    )

        }

        CardDataBaseEntry* find(CardId cardId) {
            auto it = std::find_if(
                entries.begin(),
                entries.end(),
                [cardId](const CardDataBaseEntry& e)
                {
                    return e.cardId == cardId;
                }
            );
            if(it == entries.end()) return nullptr;
            return it.base();
        }

        void check() {
            for(int i = 1; i < int(CardId::Count); i++) {
                auto cardId = static_cast<CardId>(i);
                if(!find(cardId)) {
                    std::cout << "Card id not found: " << i << "\n";
                    assert(false);
                }
            }
        }
    };

}
