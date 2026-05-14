#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class PlayableType {
        None, Playable, Unplayable, Conditional
    };
    std::string ToString(PlayableType e) { return ToString(int(e)); }

}
