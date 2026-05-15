#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EventType {
        OnDamageTaken,
        OnTurnStart,
        OnTurnEnd,
        OnKill,
        OnCardPlayed,
        Count
    };

}
