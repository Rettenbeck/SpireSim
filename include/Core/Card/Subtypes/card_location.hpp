#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class CardLocation {
        None, Deck, Hand, Discard, Exhaust, Removed, Stack, Temp
    };
    std::string ToString(CardLocation e) { return ToString(int(e)); }

}
