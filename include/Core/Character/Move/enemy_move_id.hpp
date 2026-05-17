#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EnemyMoveId {
        None,
        
        // Overgrowth
        
        // Overgrowth - Normal
            /* Shrinker Beetle */               Shrinker, Chomp, Stomp,
            /* Fuzzy Worm Crawler */            AcidGoop, Inhale,
            /* Nibbit */                        Butt, Slice, Hiss,
        Count
    };
    std::string ToString(EnemyMoveId e) { return ToString(int(e)); }
    using EnemyMoveIds = std::vector<EnemyMoveId>;

}
