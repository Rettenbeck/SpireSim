#pragma once

#include <Core/Buff/buff_id.hpp>
#include <Core/Combat/entity.hpp>


namespace SpireSim {

    const int BUFF_MAX_AMOUNT_PARAMS = 2;

    struct Buff {
        BuffId buffId;
        int params[BUFF_MAX_AMOUNT_PARAMS];
        Id createdBy = ENTITY_NONE;

        EventList eventList;

        Buff() : buffId(BuffId::None) {}
        Buff(BuffId buffId_) : buffId(buffId_) {}
        Buff(BuffId buffId_, int param) : buffId(buffId_) {
            params[0] = param;
        }
        Buff(BuffId buffId_, int param1, int param2) : buffId(buffId_) {
            params[0] = param1;
            params[1] = param2;
        }

        std::string toString() {
            std::stringstream ss;
            ss << "BuffId: " << ToString(buffId) << "; Params: { ";
            for(int i = 0; i < BUFF_MAX_AMOUNT_PARAMS; i++) ss << params[i] << "; ";
            ss << "}";
            ss << "; createdBy: " << createdBy;
            return ss.str();
        }
    };
    using Buffs = std::vector<Buff>;

}
