#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class TargetingType {
        None, Single, All, Random
    };
    std::string ToString(TargetingType e) { return ToString(int(e)); }

}
