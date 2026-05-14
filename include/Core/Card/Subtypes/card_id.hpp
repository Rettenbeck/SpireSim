#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class CardId {
        None, Strike, Defend, Bash, Thunderclap, MakeItSo,
        Count
    };
    std::string ToString(CardId e) { return ToString(int(e)); }

    std::string CardIdToString(CardId cardId) {
        switch (cardId) {
            case CardId::Strike         : return "Strike";
            case CardId::Defend         : return "Defend";
            case CardId::Bash           : return "Bash";
            case CardId::Thunderclap    : return "Thunderclap";
            case CardId::MakeItSo       : return "MakeItSo";
            default: assert(false);
        }
    }

}
