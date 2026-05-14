#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EnemyId {
        None, ShrinkerBeetle, FuzzyWormCrawler,
        Count
    };
    std::string ToString(EnemyId e) { return ToString(int(e)); }
    using EnemyIds = std::vector<EnemyId>;

}
