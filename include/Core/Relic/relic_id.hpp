#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class RelicId {
        None, BagOfPreparation, CentennialPuzzle,
        Count
    };
    std::string ToString(RelicId e) { return ToString(int(e)); }
    using RelicIds = std::vector<RelicId>;

}
