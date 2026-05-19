#pragma once

#include <Algorithm/Simulator/pool_container.hpp>


namespace SpireSim {

    struct CombatTemplate {
        PoolContainer &container;
        UCombats combats;

        Cards deck;
        Player player;
        Enemies enemies;
        Relics relics;
        Potions potions;

        bool enemiesMoreHp = false;     // Ascension 7
        bool enemiesDeadlier = false;   // Ascension 9

        unsigned int seed = 0;
        std::mt19937 rng;

        CombatTemplate(PoolContainer &container_) : container(container_) {}

        virtual Combat* get() = 0;

        CombatTemplate& setCards(Cards &deck_) {
            deck = deck_; return *this;
        }

        CombatTemplate& add(CardId cardId, bool upgraded) {
            deck.push_back(container.cardPool->createCardFromTemplate(cardId, upgraded));
            return *this;
        }

        CombatTemplate& add(const std::pair<CardId, bool> card) {
            deck.push_back(container.cardPool->createCardFromTemplate(card.first, card.second));
            return *this;
        }

        CombatTemplate& add(const std::vector<std::pair<CardId, bool>> cards) {
            for(auto [cardId, upgraded] : cards)
                deck.push_back(container.cardPool->createCardFromTemplate(cardId, upgraded));
            return *this;
        }

        CombatTemplate& setPlayer(Player &player_) {
            player = player_; return *this;
        }
        
        CombatTemplate& setPlayer(PlayerId playerId) {
            player = container.playerFactory->get(playerId);
            return *this;
        }

        CombatTemplate& setEnemies(Enemies &enemies_) {
            enemies = enemies_; return *this;
        }

        CombatTemplate& setEnemies(EncounterId encounterId) {
            enemies = container.encounterFactory->get(encounterId);
            return *this;
        }

        CombatTemplate& setRelics(Relics &relics_) {
            relics = relics_; return *this;
        }

        CombatTemplate& setRelics(const RelicIds &relicIds) {
            relics = container.relicFactory->get(relicIds);
            return *this;
        }

        CombatTemplate& setPotions(Potions &potions_) {
            potions = potions_; return *this;
        }

        CombatTemplate& setPotions(const PotionIds &potionIds) {
            potions = container.potionFactory->get(potionIds);
            return *this;
        }

        CombatTemplate& set(Cards &deck_) { return setCards(deck_); }
        CombatTemplate& set(Player &player_) { return setPlayer(player_); }
        CombatTemplate& set(PlayerId playerId) { return setPlayer(playerId); }
        CombatTemplate& set(Enemies &enemies_) { return setEnemies(enemies_); }
        CombatTemplate& set(EncounterId encounterId) { return setEnemies(encounterId); }
        CombatTemplate& set(Relics &relics_) { return setRelics(relics_); }
        CombatTemplate& set(const RelicIds &relicIds) { return setRelics(relicIds); }
        CombatTemplate& set(Potions &potions_) { return setPotions(potions_); }
        CombatTemplate& set(const PotionIds &potionIds) { return setPotions(potionIds); }

        void setSeed(unsigned int seed_) {
            seed = seed_;
            rng.seed(seed);
        }
        
        int getRandomNumber(int max) { // Including max! E.g. max = 3 produces numbers ranging from 0 to 3 including
            std::uniform_int_distribution<int> dist(0, max);
            return dist(rng);
        }

    };
    using UCombatTemplate = std::unique_ptr<CombatTemplate>;
    using UCombatTemplates = std::vector<UCombatTemplate>;

}
