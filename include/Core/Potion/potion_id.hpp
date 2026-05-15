#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class PotionId {
        None, FirePotion, FlexPotion,
        Count
    };
    std::string ToString(PotionId e) { return ToString(int(e)); }
    using PotionIds = std::vector<PotionId>;

}
