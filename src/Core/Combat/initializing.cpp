#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    CombatState::CombatState(  
                                EffectPool &effectPool_,
                                CardPool &cardPool_,
                                BuffPool &buffPool_,
                                EnemyMovePool &movePool_,
                                EnemyPool &enemyPool_,
                                const Player &player_,
                                const Enemies &enemies_,
                                Cards &cards
                            ) :
                                effectPool(effectPool_),
                                cardPool(cardPool_),
                                buffPool(buffPool_),
                                movePool(movePool_),
                                enemyPool(enemyPool_)
    {
        initializeCards(cards);
        ecs.registerPlayer(player_);
        registerEnemies(enemies_);
        stack.clear();
    }

    void CombatState::registerCardEvents(Id id, CardId cardId) {
        auto& cardTemplate = cardPool.retrieve(cardId);
        for(int i = 0; i < int(EventType::Count); i++) {
            for(auto& event : cardTemplate.eventList[i]) {
                event.effect.sourceEntityId = id;
                eventRegistry[i].push_back(EventListener(id, event.effect));
            }
        }
    }

    void CombatState::initializeCards(Cards &cards) {
        pileHandler.clearAllPiles();
        for(auto& card : cards) {
            auto id = ecs.addObject(card);
            auto& cardCreated = ecs.cCards[id];
            cardCreated.location = CardLocation::Deck;
            cardCreated.locationIndex = pileHandler.deck.size();
            pileHandler.deck.push_back(id);
            registerCardEvents(id, cardCreated.cardId);
        }
    }

    void CombatState::registerEnemies(const Enemies &enemies) {
        ecs.registerEnemies(enemies);
        for(auto& enemy : enemies) if(!enemy.isMinion) variables.enemies++;
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
