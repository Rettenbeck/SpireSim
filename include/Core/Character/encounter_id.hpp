#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EncounterId {
        None, ShrinkerBeetleWeak, FuzzyWormCrawlerWeak, SF_Test,
        Count
    };
    std::string ToString(EncounterId e) { return ToString(int(e)); }

}
