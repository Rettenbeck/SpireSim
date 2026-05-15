#pragma once

#include <Core/Relic/relic_id.hpp>
#include <Core/Event/event_listener.hpp>


namespace SpireSim {

    const int RELIC_MAX_AMOUNT_PARAMS = 2;

    struct Relic {
        RelicId relicId = RelicId::None;
        int params[RELIC_MAX_AMOUNT_PARAMS];

        EventList eventList;

        Relic() {}

        std::string toString() {
            std::stringstream ss;
            ss << "RelicId: " << ToString(relicId) << "; param1: " << params[0] << "; param1: " << params[1]
                << "; Events: " << eventList.size();
            return ss.str();
        }
    };
    using Relics = std::vector<Relic>;

}
