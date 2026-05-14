#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::moveCard(Id cardEntityId, CardLocation targetLocation) {
        pileHandler.moveCard(ecs, cardEntityId, targetLocation);
    }
    
    void CombatState::drawCard() {
        pileHandler.drawCard(ecs);
    }
    
    void CombatState::drawCards(int amount) {
        for(int i = 0; i < amount; i++) drawCard();
    }

    void CombatState::discardById(Id cardEntityId) {
        pileHandler.moveCard(ecs, cardEntityId, CardLocation::Discard);
    }

    void CombatState::discardByIndex(int index) {
        assert(pileHandler.hand.size() > index);
        discardById(pileHandler.hand[index]);
    }

    void CombatState::discardHand() {
        while(pileHandler.hand.size() > 0) {
            discardByIndex(0);
        }
    }

    int CombatState::getRandomNumber(int max) { // Inlcuding max! e.g. max = 3 produces number ranging from 0 to 3 including
        std::uniform_int_distribution<int> dist(0, max);
        return dist(genDeck);
    }

}
