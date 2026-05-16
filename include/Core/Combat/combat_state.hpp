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

        unsigned int seedDeck = 0;
        std::mt19937 genDeck;
    };

}
