#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectId {
        ChooseCardsFromDeck, ChooseCardsFromDiscard,
        MoveChosenCardsToDeck,
        CardDealDamage, CardGainBlock, CardApplyVulnerable,
    };
    std::string ToString(EffectId e) { return ToString(int(e)); }

}
