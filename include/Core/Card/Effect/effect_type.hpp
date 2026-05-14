#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectType {
        None = 0, UnpackCard = 1099, FinishCardPlayed = 1100,
        CardDealDamage = 1101, CardGainBlock = 1102, CardApplyVulnerable = 1103,
        MoveCard = 1104
    };
    std::string ToString(EffectType e) { return ToString(int(e)); }

}
