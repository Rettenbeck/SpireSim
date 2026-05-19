#pragma once

#include <Core/Relic/relic_id.hpp>
#include <Core/Event/event_listener.hpp>


namespace SpireSim {

    const int RELIC_MAX_AMOUNT_PARAMS = 2;

    struct Relic {
        RelicId relicId = RelicId::None;
        int params[RELIC_MAX_AMOUNT_PARAMS] = {0, 0};

        EventList eventList;

        Relic() {}

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Relic, relicId, params, eventList);
        TO_STRING_METHOD
    
        // std::string toString() {
        //     std::stringstream ss;
        //     ss << "RelicId: " << ToString(relicId) << "; param1: " << params[0] << "; param1: " << params[1]
        //         << "; Events: " << eventList.size();
        //     return ss.str();
        // }
    };
    using Relics = std::vector<Relic>;

}
