#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EventType {
        OnCardPlayed,
        OnDamageTaken,
        OnTurnStart,
        OnTurnEnd,
        OnKill,
        OnDealDamageForInterception,
        Count
    };

}
