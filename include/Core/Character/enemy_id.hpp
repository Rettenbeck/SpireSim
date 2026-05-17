#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EnemyId {
        None,

        // Overgrowth
        
        // Overgrowth - Normal
        ShrinkerBeetle, FuzzyWormCrawler, Nibbit,

        Count
    };
    std::string ToString(EnemyId e) { return ToString(int(e)); }
    using EnemyIds = std::vector<EnemyId>;

}
