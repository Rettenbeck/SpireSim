#pragma once

#include <Core/Combat/entity.hpp>
#include <Core/Card/Subtypes/card_id.hpp>


namespace SpireSim {

    const int TARGET_RANDOM = -1;
        
    struct CombatVariables {
        bool enemiesMoreHp = false;     // Ascension 7
        bool enemiesDeadlier = false;   // Ascension 9
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
        int clawDamage = 0;
        int cardAddedDamage = 0;
        int cardHits = -1;
        
        bool recordDamage = false;
        std::map<Id, std::map<Id, std::vector<int>>> entityDamageRecorded; // [from][to][damage]

        Ids chosenCards, choosableCards;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(CombatVariables,
            enemiesMoreHp, enemiesDeadlier,
            initialHp, turn, cardDrawOnTurn, maxHandSize, energyPerTurn, energy, stars,
            hasTakenDamage, cardsPlayedThisCombat, xValue, enemies,
            clawDamage,
            chosenCards, choosableCards
        );
        TO_STRING_METHOD
    
        // std::string toString() {
        //     std::stringstream ss;
        //     ss << "energy: " << energy;
        //     ss << "; stars: " << stars;
        //     ss << "; cardsPlayedThisCombat: " << cardsPlayedThisCombat;
        //     return ss.str();
        // }
    };

}
