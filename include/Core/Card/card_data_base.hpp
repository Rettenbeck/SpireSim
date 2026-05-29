#pragma once

#include <Core/Card/Subtypes/card_id.hpp>
#include <Core/Card/Subtypes/card_rarity.hpp>
#include <Core/Character/player_id.hpp>


namespace SpireSim {

    #define ADDC(NAME,RARITY,PLAYER) \
        entries.push_back(CardDataBaseEntry(CardId::NAME, CardRarity::RARITY, PlayerId::PLAYER, #NAME));

    #define ADDF(NAME,RARITY,PLAYER,IS_STRIKE,IS_DEFEND) \
        entries.push_back(CardDataBaseEntry(CardId::NAME, CardRarity::RARITY, PlayerId::PLAYER, #NAME, IS_STRIKE, IS_DEFEND));

    struct CardDataBaseEntry {
        CardId cardId = CardId::None;
        CardRarity rarity = CardRarity::None;
        PlayerId playerId = PlayerId::None;
        std::string name = "NONE";
        bool isStrike = false;
        bool isDefend = false;

        CardDataBaseEntry() {}

        CardDataBaseEntry(CardId cardId_, CardRarity rarity_, PlayerId playerId_, std::string name_)
            : cardId(cardId_), rarity(rarity_), playerId(playerId_), name(name_) {}
        
        CardDataBaseEntry(CardId cardId_, CardRarity rarity_, PlayerId playerId_,
            std::string name_, bool isStrike_, bool isDefend_) :
                cardId(cardId_), rarity(rarity_), playerId(playerId_),
                name(name_), isStrike(isStrike_), isDefend(isDefend_) {}
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
            ADDF(Strike                     , Basic     , Colorless , true  , false )
            ADDF(Defend                     , Basic     , Colorless , false , true  )

            // Ironclad
            ADDC(Bash                       , Basic     , IronClad  )
            ADDC(Thunderclap                , Common    , IronClad  )
            ADDC(BloodLetting               , Common    , IronClad  )

            // Silent
            ADDC(Neutralize                 , Basic     , Silent    )

            // Regent
            ADDC(Venerate                   , Basic     , Regent    )
            ADDC(FallingStar                , Basic     , Regent    )
            ADDC(AstralPulse                , Common    , Regent    )
            ADDC(CosmicIndifference         , Common    , Regent    )
            ADDC(MakeItSo                   , Rare      , Regent    )

            // Defect
            ADDC(Hologram                   , Common    , Defect    )
            ADDC(Claw                       , Common    , Defect    )

        }

        CardDataBaseEntries::iterator find(CardId cardId) {
            return std::find_if(
                entries.begin(),
                entries.end(),
                [cardId](const CardDataBaseEntry& e)
                {
                    return e.cardId == cardId;
                }
            );
        }

        void check() {
            for(int i = 1; i < int(CardId::Count); i++) {
                auto cardId = static_cast<CardId>(i);
                auto it = find(cardId);
                if(it == entries.end()) {
                    std::cout << "Card id not found: " << i << "\n";
                    assert(false);
                }
            }
        }
    };

}
