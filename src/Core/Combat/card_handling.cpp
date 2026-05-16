#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    void Combat::moveCard(Id cardEntityId, CardLocation targetLocation) {
        pileHandler.moveCard(ecs, cardEntityId, targetLocation);
    }
    
    void Combat::drawCard() {
        pileHandler.drawCard(ecs);
    }
    
    void Combat::drawCards(int amount) {
        for(int i = 0; i < amount; i++) drawCard();
    }

    void Combat::discardById(Id cardEntityId) {
        pileHandler.moveCard(ecs, cardEntityId, CardLocation::Discard);
    }

    void Combat::discardByIndex(int index) {
        assert(pileHandler.hand.size() > index);
        discardById(pileHandler.hand[index]);
    }

    void Combat::discardHand() {
        while(pileHandler.hand.size() > 0) {
            discardByIndex(0);
        }
    }

    void Combat::determineChoosableCards(CardLocation from) {
        variables.choosableCards.clear();
        auto& v = variables.chosenCards;
        for(auto cardEntityId : pileHandler.locationToPile(from)) {
            if(std::find(v.begin(), v.end(), cardEntityId) != v.end()) continue;
            variables.choosableCards.push_back(cardEntityId);
        }
    }

    void Combat::chooseCard() {
        state.actions.clear();
        for(auto cardEntityId : variables.choosableCards) {
            state.actions.push_back(Action(ActionType::ChooseCard, cardEntityId));
        }
        if(state.actions.empty()) {
            state.waitingForActionOnStack = false;
            resolveStackFully();
        }
    }

    int Combat::getRandomNumber(int max) { // Inlcuding max! e.g. max = 3 produces number ranging from 0 to 3 including
        std::uniform_int_distribution<int> dist(0, max);
        return dist(state.genDeck);
    }

}
