#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class ParamType {
        None, FixedValue,
        CardProperty, CardAttackDamage, CardHits, CardBlockValue,
        CardLoseLive,
        CardApplyVulnerable, CardApplyWeak,
        CardsPlayedThisCombat,
        CardAnyParam1, CardAnyParam2, CardAnyParam3,
        XValue,
        ClawDamage
    };
    std::string ToString(ParamType e) { return ToString(int(e)); }

}
