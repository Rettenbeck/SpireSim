#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectType {
        None, UnpackCard, FinishCardPlayed,
        CardDealDamage, CardGainBlock, CardApplyVulnerable,
        MoveCard,
        ModifyParentDamagePerc
    };
    std::string ToString(EffectType e) { return ToString(int(e)); }

}
