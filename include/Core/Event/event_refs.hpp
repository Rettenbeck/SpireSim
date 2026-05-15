#pragma once

#include <Core/Combat/entity.hpp>
#include <Core/Card/Effect/effect.hpp>
#include <Core/Event/event_type.hpp>


namespace SpireSim {

    struct EventRef {
        int eventType, eventIndex;
        EventRef() {}
        EventRef(int eventType_, int eventIndex_) : eventType(eventType_), eventIndex(eventIndex_) {}

        std::string toString() {
            std::stringstream ss;
            ss << "type: " << eventType << "; index: " << eventIndex;
            return ss.str();
        }
    };

    struct EventRefList {
        std::vector<EventRef> list;
        EventRefList() {}

        std::string toString() {
            std::stringstream ss;
            ss << "Event ref list: { ";
            for(auto& entry : list) {
                ss << "[" << entry.toString() << "] ";
            }
            ss << "}";
            return ss.str();
        }
    };

    using EventRefs = std::vector<EventRefList>;

    std::string ToString(EventRefs &refs) {
        std::stringstream ss;
        ss << "Event refs: { ";
        for(auto& list : refs) {
            ss << list.toString() << " ";
        }
        ss << "}";
        return ss.str();
    }

}
