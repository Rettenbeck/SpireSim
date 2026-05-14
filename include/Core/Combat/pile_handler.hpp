#pragma once

#include <Core/Combat/ecs.hpp>
#include <Core/Card/Subtypes/card_location.hpp>


namespace SpireSim {

    using Pile = std::vector<int>;
    const bool KEEP_ORDER = false;

    struct PileHandler {
        Pile deck, hand, discard, exhaust, removed, stack, temp;

        unsigned int seedDeck = 0;
        std::mt19937 genDeck;

        int getRandomNumber(int max) { // Inlcuding max! e.g. max = 3 produces number ranging from 0 to 3 including
            std::uniform_int_distribution<int> dist(0, max);
            return dist(genDeck);
        }

        void refreshPile(ECS &ecs, Pile &pile) {
            int index = 0;
            for(auto cardEntityId : pile) {
                ecs.getCard(cardEntityId).locationIndex = index;
                index++;
            }
        }
        
        inline Pile& locationToPile(CardLocation location) {
            switch(location) {
                case CardLocation::Deck   : return deck   ;
                case CardLocation::Hand   : return hand   ;
                case CardLocation::Discard: return discard;
                case CardLocation::Exhaust: return exhaust;
                case CardLocation::Removed: return removed;
                case CardLocation::Stack  : return stack  ;
                case CardLocation::Temp   : return temp   ;
                default: assert(false);
            }
        }
        
        void moveCard(ECS &ecs, Id cardEntityId, CardLocation targetLocation) {
            auto& card = ecs.getCard(cardEntityId);
            auto& source = locationToPile(card.location);
            auto& target = locationToPile(targetLocation);
            
            switch(KEEP_ORDER) {
                case true:
                    source.erase(source.begin() + card.locationIndex);
                    refreshPile(ecs, source);
                    break;
                case false:
                    // Swap-and-Pop statt erase
                    Id lastId = source.back();
                    source[card.locationIndex] = lastId; // Letztes Element an die Stelle des gelöschten
                    ecs.getCard(lastId).locationIndex = card.locationIndex; // Index des verschobenen Elements updaten
                    source.pop_back(); // Letztes Element entfernen (O(1))
                    break;
            }

            card.location = targetLocation;
            card.locationIndex = target.size();
            target.push_back(cardEntityId);
        }

        void moveLastCardOfPile(ECS &ecs, CardLocation sourceLocation, CardLocation targetLocation) {
            auto& source = locationToPile(sourceLocation);
            auto& target = locationToPile(targetLocation);

            Id lastId = source.back();
            auto& card = ecs.getCard(lastId);
            source.pop_back();

            card.location = targetLocation;
            card.locationIndex = target.size();
            target.push_back(lastId);
        }

        void movePile(ECS &ecs, CardLocation sourceLocation, CardLocation targetLocation) {
            for(auto& id : locationToPile(sourceLocation)) moveCard(ecs, id, targetLocation);
        }

        void discardHand(ECS &ecs) {
            movePile(ecs, CardLocation::Hand, CardLocation::Discard);
        }

        void shufflePileIntoDeck(ECS &ecs, CardLocation sourceLocation) {
            auto& source = locationToPile(sourceLocation);
            while(source.size() > 0) {
                int index = (source.size() == 1) ? 0 : getRandomNumber(source.size() - 1);
                moveCard(ecs, source[index], CardLocation::Deck);
            }
        }

        void reshuffleDeck(ECS &ecs) {
            movePile(ecs, CardLocation::Deck, CardLocation::Temp);
            shufflePileIntoDeck(ecs, CardLocation::Temp);
        }

        void shuffleDiscardIntoDeck(ECS &ecs) {
            shufflePileIntoDeck(ecs, CardLocation::Discard);
        }

        void drawCard(ECS &ecs, int maxHandSize = 10) {
            if(deck.size() == 0) {
                shuffleDiscardIntoDeck(ecs);
            }
            if(deck.size() == 0) return;
            if(hand.size() < maxHandSize) {
                moveLastCardOfPile(ecs, CardLocation::Deck, (hand.size() >= maxHandSize) ? CardLocation::Discard : CardLocation::Hand);
            }
        }

        void drawCardSpecific(ECS &ecs, int maxHandSize = 10, int index = 0) {
            // if(deck.size() == 0) return;
            // if(hand.size() < maxHandSize) {
            //     moveCard(ecs, deck[index], (hand.size() >= maxHandSize) ? CardLocation::Discard : CardLocation::Hand);
            // }
        }
        
        void clearAllPiles() {
            deck.clear(); hand.clear(); discard.clear();
            exhaust.clear(); removed.clear(); stack.clear();
        }
        
        std::string pileToString(std::string description, Pile &pile) {
            std::stringstream ss;
            ss << description << ": ";
            for(auto i : pile) ss << i << " ";
            ss << " (" << pile.size() << ")";
            return ss.str();
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "\n";
            ss << pileToString("  Deck", deck) << "\n";
            ss << pileToString("  Hand", hand) << "\n";
            ss << pileToString("  Discard", discard) << "\n";
            ss << pileToString("  Exhaust", exhaust) << "\n";
            ss << pileToString("  Removed", removed) << "\n";
            ss << pileToString("  Stack", stack);
            return ss.str();
        }
    };

}
