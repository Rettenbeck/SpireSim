#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    Combat::Combat(  
                    EffectPool &effectPool_,
                    RelicPool &relicPool_,
                    PotionPool &potionPool_,
                    CardPool &cardPool_,
                    BuffPool &buffPool_,
                    EnemyMovePool &movePool_,
                    EnemyPool &enemyPool_,
                    const Player &player_,
                    const Enemies &enemies_,
                    const Relics &relics_,
                    const Potions &potions_,
                    Cards &cards
                  ) :
                    effectPool(effectPool_),
                    relicPool(relicPool_),
                    potionPool(potionPool_),
                    cardPool(cardPool_),
                    buffPool(buffPool_),
                    movePool(movePool_),
                    enemyPool(enemyPool_)
    {
        initializeCards(cards);
        ecs.registerPlayer(player_);
        registerEnemies(enemies_);
        registerRelics(relics_);
        registerPotions(potions_);
    }

    void Combat::initialize(bool shuffleDeck) {
        if(shuffleDeck) pileHandler.reshuffleDeck(ecs);
        variables.initialHp = ecs.getPlayer().data.hp;
        state.stack.clear();
    }

    void Combat::initializeCards(Cards &cards) {
        pileHandler.clearAllPiles();
        for(auto& card : cards) {
            auto id = ecs.addObject(card);
            auto& cardCreated = ecs.cCards[id];
            cardCreated.location = CardLocation::Deck;
            cardCreated.locationIndex = pileHandler.deck.size();
            pileHandler.deck.push_back(id);
            registerEventsFromEntity(cardPool.retrieve(cardCreated.cardId), id);
        }
    }

    void Combat::registerEnemies(const Enemies &enemies) {
        ecs.registerEnemies(enemies);
        for(auto& enemy : enemies) if(!enemy.isMinion) variables.enemies++;
    }

    void Combat::registerRelics(const Relics &relics) {
        for(auto& relic : relics) {
            auto id = ecs.addObject(relic);
            auto relicCopy = relic;
            auto eventList = relic.eventList;
            registerEventsFromList(eventList, id);
        }
    }

    void Combat::registerPotions(const Potions &potions) {
        for(auto& potion : potions) {
            auto id = ecs.addObject(potion);
        }
    }

    void Combat::setSeeds(unsigned int seedDeck_) {
        state.seedDeck = seedDeck_;
        state.genDeck.seed(state.seedDeck);

        pileHandler.seedDeck = seedDeck_;
        pileHandler.genDeck.seed(pileHandler.seedDeck);
    }

    void Combat::increaseSeeds(unsigned int value) {
        setSeeds(state.seedDeck + value);
    }

}
