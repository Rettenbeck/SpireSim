#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EffectId {
        CardDealDamage, CardGainBlock, CardApplyVulnerable
    };
    std::string ToString(EffectId e) { return ToString(int(e)); }

}
