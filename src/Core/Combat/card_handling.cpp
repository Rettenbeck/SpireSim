#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    void Combat::moveCard(Id cardEntityId, CardLocation targetLocation) {
        pileHandler.moveCard(ecs, cardEntityId, targetLocation);
    }
    
    void Combat::drawCard() {
        auto cardEntityId = pileHandler.drawCard(ecs);
        state.cardsDrawnMap[cardEntityId]++;
    }
    
    void Combat::drawCards(int amount) {
        for(int i = 0; i < amount; i++) drawCard();
    }

    void Combat::discardById(Id cardEntityId) {
        pileHandler.moveCard(ecs, cardEntityId, CardLocation::Discard);
        state.cardsDiscardedMap[cardEntityId]++;
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

    void Combat::discardHandEndOfTurn() {
        for(std::size_t i = pileHandler.hand.size(); i-- > 0; ) {
            assert(pileHandler.hand.size() > i);
            auto cardEntityId = pileHandler.hand[i];
            auto& card = ecs.getCard(cardEntityId);
            if(!card.data.retain) discardById(cardEntityId);
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

    Id Combat::createCardInPile(CardLocation location, CardId cardId, bool isUpgraded) {
        if(location == CardLocation::Hand && pileHandler.hand.size() >= variables.maxHandSize) {
            location = CardLocation::Discard;
        }
        
        Card card;
        card.isUpgraded = isUpgraded;
        cardPool.retrieve(cardId).derive(card);
        auto id = ecs.addObject(card);
        Card& cardCreated = ecs.cCards[id];
        cardCreated.location = location;
        auto& pile = pileHandler.locationToPile(location);
        cardCreated.locationIndex = pile.size();
        pile.push_back(id);
        registerEventsFromEntity(cardPool.retrieve(cardCreated.cardId), id);
        return id;
    }

    Id Combat::transform(Id cardEntityId, CardId cardId, bool isUpgraded) {
        auto& cardOld = ecs.getCard(cardEntityId);
        unregisterEventsFromEntity(cardEntityId);
        auto location = cardOld.location;
        auto locationIndex = cardOld.locationIndex;
        cardOld.location = CardLocation::Removed;
        cardOld.locationIndex = pileHandler.removed.size();
        pileHandler.removed.push_back(cardEntityId);
        
        
        Card newCard;
        newCard.isUpgraded = isUpgraded;
        cardPool.retrieve(cardId).derive(newCard);
        auto newCardEntityId = ecs.addObject(newCard);
        Card& cardCreated = ecs.cCards[newCardEntityId];
        cardCreated.location = location;
        cardCreated.locationIndex = locationIndex;
        pileHandler.locationToPile(cardCreated.location)[locationIndex] = newCardEntityId;
        registerEventsFromEntity(cardPool.retrieve(cardCreated.cardId), newCardEntityId);

        return newCardEntityId;
    }
    
}
