#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class PlayerId {
        None, IronClad, Silent, Regent, NecroBinder, Defect,
        Count
    };
    std::string ToString(PlayerId e) { return ToString(int(e)); }

}
