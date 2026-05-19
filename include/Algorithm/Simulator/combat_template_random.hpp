#pragma once

#include <Algorithm/Simulator/combat_template.hpp>


namespace SpireSim {

    struct CombatTemplateRandom : public CombatTemplate {

        int optionMinDeckSize = 10;
        int optionMaxDeckSize = 15;
        int optionMinHpPercentage = 40;
        int optionMaxHpPercentage = 100;

        PlayerIds playerIds;
        EncounterIds encounterIds;

        CombatTemplateRandom(PoolContainer &container_) : CombatTemplate(container_) {}
        
        void create() {
            combats.clear();

            assert(optionMinDeckSize > 0);
            assert(optionMinDeckSize <= optionMaxDeckSize);
            assert(optionMinHpPercentage > 0);
            assert(optionMaxHpPercentage <= 100);
            assert(optionMinHpPercentage <= optionMaxHpPercentage);

            assert(deck.size() > 0);
            assert(playerIds.size() > 0);
            assert(encounterIds.size() > 0);

            int deckSize = getRandomNumber(optionMaxDeckSize - optionMinDeckSize) + optionMinDeckSize;
            Cards finalDeck;
            for(int i = 0; i < deckSize; i++) {
                int index = getRandomNumber(deck.size() - 1);
                finalDeck.push_back(deck[index]);
            }

            Relics finalRelics;
            for(auto& relic : relics) {
                if(getRandomNumber(1) == 0) finalRelics.push_back(relic);
            }

            Potions finalPotions;
            for(auto& potion : potions) {
                if(getRandomNumber(1) == 0) finalPotions.push_back(potion);
            }

            auto playerId = playerIds[getRandomNumber(playerIds.size() - 1)];
            auto encounterId = encounterIds[getRandomNumber(encounterIds.size() - 1)];

            auto finalPlayer = container.playerFactory->get(playerId);
            auto finalEnemies = container.encounterFactory->get(encounterId);

            float hpPercentage = getRandomNumber(optionMaxHpPercentage - optionMinHpPercentage) + optionMinHpPercentage;
            float hpMax = finalPlayer.data.maxHp;
            hpMax = hpMax * hpPercentage / 100.f;
            finalPlayer.data.hp = hpMax;

            combats.push_back(
                std::make_unique<SpireSim::Combat>(
                    *container.effectPool, *container.relicPool, *container.potionPool,
                    *container.cardPool, *container.buffPool, *container.movePool, *container.enemyPool,
                    finalPlayer, finalEnemies, finalRelics, finalPotions, finalDeck
                )
            );
        }

        Combat* get() {
            create();
            assert(combats.size() <= 1);
            return combats[0].get();
        }

        CombatTemplateRandom& set(const PlayerIds &playerIds_) {
            playerIds = playerIds_;
            return *this;
        }

        CombatTemplateRandom& set(const EncounterIds &encounterIds_) {
            encounterIds = encounterIds_;
            return *this;
        }

        CombatTemplateRandom& set(const RelicIds &relicIds) {
            relics = container.relicFactory->get(relicIds);
            return *this;
        }

        CombatTemplateRandom& set(const PotionIds &potionIds) {
            potions = container.potionFactory->get(potionIds);
            return *this;
        }

    };

}
