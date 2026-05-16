#pragma once

#include <Core/Combat/entity.hpp>


namespace SpireSim {

    const int TARGET_RANDOM = -1;
        
    struct CombatVariables {
        int initialHp = 0;
        int turn = 0;
        int cardDrawOnTurn = 5;
        int maxHandSize = 10;
        int energyPerTurn = 3;
        int energy = 0;
        int stars = 0;
        int hasTakenDamage = 0;
        int cardsPlayedThisCombat = 0;
        int xValue = 0;
        int enemies = 0;
        
        Ids chosenCards;

        std::string toString() {
            std::stringstream ss;
            ss << "energy: " << energy;
            ss << "; stars: " << stars;
            ss << "; cardsPlayedThisCombat: " << cardsPlayedThisCombat;
            return ss.str();
        }
    };

}
