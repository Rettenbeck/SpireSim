#pragma once

#include <Core/Potion/potion_id.hpp>
#include <Core/Event/event_listener.hpp>


namespace SpireSim {

    struct Potion {
        PotionId potionId = PotionId::None;
        // EventList eventList;

        Potion() {}

        std::string toString() {
            std::stringstream ss;
            ss << "PotionId: " << ToString(potionId);
            return ss.str();
        }
    };
    using Potions = std::vector<Potion>;

}
