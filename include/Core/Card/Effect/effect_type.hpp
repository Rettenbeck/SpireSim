#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectType {
        None, UnpackCard, FinishCardPlayed,
        ChooseCards, ChooseSingleCard,
        DrawCards, GainEnergy, GainStars,
        CardDealDamage, CardGainBlock, CardApplyVulnerable, CardApplyWeak,
        LoseLifeSelf,
        MoveCard, MoveChosenCards,
        ModifyParentDamagePerc
    };
    std::string ToString(EffectType e) { return ToString(int(e)); }

}
