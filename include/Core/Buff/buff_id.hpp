#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class BuffId {
        None, Shrink, GainStrengthPerTurn,
        Count
    };
    std::string ToString(BuffId e) { return ToString(int(e)); }

}
