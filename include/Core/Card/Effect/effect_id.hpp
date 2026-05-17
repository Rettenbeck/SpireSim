#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectId {
        CardDrawCards,
        ChooseCardsFromDeck, ChooseCardsFromDiscard,
        MoveChosenCardsToDeck, MoveChosenCardsToHand,
        CardGainEnergy, CardGainStars,
        CardDealDamage, CardGainBlock, CardApplyVulnerable, CardApplyWeak,
        CardLoseLifeSelf
    };
    std::string ToString(EffectId e) { return ToString(int(e)); }

}
