#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class ModificationType {
        Ethereal, Exhaust
    };
    std::string ToString(ModificationType e) { return ToString(int(e)); }

}
