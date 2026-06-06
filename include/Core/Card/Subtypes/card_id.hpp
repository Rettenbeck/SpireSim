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
        AstralPulse, Begone, CelestialMight, CloakOfStars, CollisionCourse,
        CosmicIndifference,
        Terraforming,
        Stardust,
        BigBang,
        MakeItSo,

        // Regent - Generated
        SovereignBlade, MinionStrike,
        
        // Defect
        Hologram, Claw,

        // Status
        Debris,

        Count
    };
    std::string ToString(CardId e) { return ToString(int(e)); }

}
