#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class ParamType {
        None = 0, FixedValue = 1201, CardProperty = 1200, CardAttackDamage = 1202, CardBlockValue = 1203,
        CardApplyVulnerable = 1204, CardsPlayedThisCombat = 1205, XValue = 1206
    };
    std::string ToString(ParamType e) { return ToString(int(e)); }

}
