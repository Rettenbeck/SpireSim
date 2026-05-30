#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectId {
        CardDrawCards,
        ChooseCardsFromDeck, ChooseCardsFromDiscard,
        MoveChosenCardsToDeck, MoveChosenCardsToHand,
        CardGainEnergy, CardGainStars,
        CardDealDamage, CardGainBlock, CardApplyVulnerable, CardApplyWeak,
        CardGainVigor,
        CardLoseLifeSelf,
        CardCreateCardInHand, CardCreateCardUpgradedInHand,

        // Card specific
        CardModifyDamageFlatClaw, CardSharpenClaws
    };
    std::string ToString(EffectId e) { return ToString(int(e)); }

}
