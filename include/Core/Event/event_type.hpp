#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EventType {
        AfterFirstDrawPhase,
        OnCardPlayed,
        OnFirstDamageTaken,
        OnDamageTaken,
        OnTurnStart,
        OnTurnEnd,
        OnKill,
        OnDealDamageForInterception,
        Count
    };

}
