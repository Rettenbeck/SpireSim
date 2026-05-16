#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class ActionType {
        None, EndTurn, PlayCard, UsePotion, Secondary, ChooseCard
    };
    std::string ToString(ActionType e) { return ToString(int(e)); }

}
