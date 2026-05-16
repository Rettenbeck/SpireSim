#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class ParamType {
        None, FixedValue,
        CardProperty, CardAttackDamage, CardBlockValue,
        CardApplyVulnerable, CardsPlayedThisCombat,
        CardToChoose,
        XValue
    };
    std::string ToString(ParamType e) { return ToString(int(e)); }

}
