#pragma once

#include <Core/Potion/potion_id.hpp>
#include <Core/Event/event_listener.hpp>


namespace SpireSim {

    struct Potion {
        PotionId potionId = PotionId::None;
        // EventList eventList;

        Potion() {}

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Potion, potionId);
        TO_STRING_METHOD
    
        // std::string toString() {
        //     std::stringstream ss;
        //     ss << "PotionId: " << ToString(potionId);
        //     return ss.str();
        // }
    };
    using Potions = std::vector<Potion>;

}
