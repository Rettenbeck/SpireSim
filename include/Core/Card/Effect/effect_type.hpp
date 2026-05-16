#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectType {
        None, UnpackCard, FinishCardPlayed,
        DrawCards,
        ChooseCards, ChooseSingleCard,
        CardDealDamage, CardGainBlock, CardApplyVulnerable,
        MoveCard, MoveChosenCards,
        ModifyParentDamagePerc
    };
    std::string ToString(EffectType e) { return ToString(int(e)); }

}
