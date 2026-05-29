#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class CardId {
        None,
        // Basics
        Strike, Defend,

        // Ironclad
        Bash, Thunderclap, BloodLetting,
        
        // Silent
        Neutralize,

        // Regent
        Venerate, FallingStar,
        AstralPulse, CelestialMight,
        CosmicIndifference, MakeItSo,
        
        // Defect
        Hologram, Claw,

        Count
    };
    std::string ToString(CardId e) { return ToString(int(e)); }

}
