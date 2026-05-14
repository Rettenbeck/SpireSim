#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class TurnId {
        None,
        PlayerDraw, PlayerUpkeep, Player, PlayerEnd,
        EnemyUpkeep, Enemy, EnemyEnd,
        Count
    };
    std::string ToString(TurnId e) { return ToString(int(e)); }

}
