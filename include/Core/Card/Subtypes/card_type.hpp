#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class CardType {
        Attack, Skill, Power, Status, Curse
    };
    std::string ToString(CardType e) { return ToString(int(e)); }

}
