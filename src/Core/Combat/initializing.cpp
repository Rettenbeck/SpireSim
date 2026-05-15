#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    CombatState::CombatState(  
                                EffectPool &effectPool_,
                                RelicPool &relicPool_,
                                CardPool &cardPool_,
                                BuffPool &buffPool_,
                                EnemyMovePool &movePool_,
                                EnemyPool &enemyPool_,
                                const Player &player_,
                                const Enemies &enemies_,
                                const Relics &relics_,
                                Cards &cards
                            ) :
                                effectPool(effectPool_),
                                relicPool(relicPool_),
                                cardPool(cardPool_),
                                buffPool(buffPool_),
                                movePool(movePool_),
                                enemyPool(enemyPool_)
    {
        initializeCards(cards);
        ecs.registerPlayer(player_);
        registerEnemies(enemies_);
        registerRelics(relics_);
    }

    void CombatState::initialize(bool shuffleDeck) {
        if(shuffleDeck) pileHandler.reshuffleDeck(ecs);
        variables.initialHp = ecs.getPlayer().data.hp;
        stack.clear();
    }

    void CombatState::initializeCards(Cards &cards) {
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

    void CombatState::registerEnemies(const Enemies &enemies) {
        ecs.registerEnemies(enemies);
        for(auto& enemy : enemies) if(!enemy.isMinion) variables.enemies++;
    }

    void CombatState::registerRelics(const Relics &relics) {
        for(auto& relic : relics) {
            auto id = ecs.addObject(relic);
            auto relicCopy = relic;
            auto eventList = relic.eventList;
            registerEventsFromList(eventList, id);
        }
    }

    void CombatState::setSeeds(unsigned int seedDeck_) {
        seedDeck = seedDeck_;
        genDeck.seed(seedDeck);

        pileHandler.seedDeck = seedDeck_;
        pileHandler.genDeck.seed(pileHandler.seedDeck);
    }

    void CombatState::increaseSeeds(unsigned int value) {
        setSeeds(seedDeck + value);
    }

}
