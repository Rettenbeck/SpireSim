#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class CardId {
        None,
        // Basics
        Strike, Defend,

        // Ironclad
        Bash, Thunderclap, BloodLetting,
        
        // Silent
        Neutralize,

        // Regent
        Venerate, FallingStar,
        CosmicIndifference, MakeItSo,
        
        // Defect
        Hologram, Claw,

        Count
    };
    std::string ToString(CardId e) { return ToString(int(e)); }

    // std::string CardIdToString(CardId cardId) {
    //     switch (cardId) {
    //         // case CardId::None               : return "None";
    //         case CardId::Strike             : return "Strike";
    //         case CardId::Defend             : return "Defend";
    //         case CardId::Bash               : return "Bash";
    //         case CardId::Thunderclap        : return "Thunderclap";
    //         case CardId::BloodLetting       : return "Blood Letting";
    //         case CardId::Venerate           : return "Venerate";
    //         case CardId::FallingStar        : return "Falling Star";
    //         case CardId::MakeItSo           : return "Make It So";
    //         case CardId::CosmicIndifference : return "Cosmic Indifference";
    //         case CardId::Hologram           : return "Hologram";
    //         case CardId::Claw               : return "Claw";
    //         default: assert(false);
    //     }
    // }

}
