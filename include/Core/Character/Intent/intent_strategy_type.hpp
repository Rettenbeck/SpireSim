#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class IntentStrategyType {
        None, Cycle
    };
    std::string ToString(IntentStrategyType e) { return ToString(int(e)); }

}
