#pragma once

#include <Core/Event/event_listener.hpp>
#include <Core/Combat/turn_id.hpp>
#include <Core/Combat/Action/action.hpp>


namespace SpireSim {

    struct CombatState {
        EventRegistry eventRegistry;
        
        Effects stack;

        TurnId turnId = TurnId::None;

        bool isStackRunning = false;
        bool waitingForActionOnStack = false;
        bool waitingForAction = false;
        bool stopStack = false;
        bool combatOver = false;
        Actions actions;

        std::map<Id, int> cardsDrawnMap;
        std::map<Id, int> cardsDiscardedMap;

        RNG rngDeck, rngEnemies, rngCast;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CombatState,
            eventRegistry, stack, turnId,
            isStackRunning, waitingForActionOnStack, waitingForAction, stopStack, combatOver, actions
        );
        TO_STRING_METHOD
    
    };

}
