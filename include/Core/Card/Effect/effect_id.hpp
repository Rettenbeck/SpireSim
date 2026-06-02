#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectId {
        CardDrawCards,
        ChooseCardsFromDeck, ChooseCardsFromHand, ChooseCardsFromDiscard,
        MoveChosenCardsToDeck, MoveChosenCardsToHand,
        CardGainEnergy, CardGainStars,
        CardDealDamage, CardGainBlock, CardApplyVulnerable, CardApplyWeak,
        CardMultiHitFixed, CardMultiHitX,
        CardGainVigor,
        CardLoseLifeSelf,
        CardCreateCardInHand, CardCreateCardUpgradedInHand,
        CardTransformChosen, CardTransformChosenUpgraded,

        // Card specific
        CardModifyDamageFlatClaw, CardSharpenClaws
    };
    std::string ToString(EffectId e) { return ToString(int(e)); }

}
