#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectType {
        None, UnpackCard, FinishCardPlayed,
        ChooseCards, ChooseSingleCard,
        DrawCards, GainEnergy, GainStars,
        CardDealDamage, CardGainBlock, CardApplyVulnerable, CardApplyWeak,
        CardMultiHit,
        GainVigor,
        LoseLifeSelf,
        MoveCard, MoveChosenCards,
        ModifyParentDamagePerc, CardModifyDamageFlat,
        CreateCardInHand, CreateCardUpgradedInHand,
        TransformChosen, TransformChosenUpgraded,

        // Card specific
        SharpenClaws
    };
    std::string ToString(EffectType e) { return ToString(int(e)); }

}
