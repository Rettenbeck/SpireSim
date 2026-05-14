#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EnemyMoveId {
        None,
        /* Shrinker Beetle */               Shrinker, Chomp, Stomp,
        /* Fuzzy Worm Crawler */            AcidGoop, Inhale,
        Count
    };
    std::string ToString(EnemyMoveId e) { return ToString(int(e)); }
    using EnemyMoveIds = std::vector<EnemyMoveId>;

}
