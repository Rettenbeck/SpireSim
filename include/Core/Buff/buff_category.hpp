#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class BuffCategory {
        None, Buff, Debuff, Other
    };
    std::string ToString(BuffCategory e) { return ToString(int(e)); }

}
