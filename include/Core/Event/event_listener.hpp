#pragma once

#include <Core/Combat/entity.hpp>
#include <Core/Card/Effect/effect.hpp>
#include <Core/Event/event_type.hpp>


namespace SpireSim {

    struct EventListener {
        Id entityId = ENTITY_NONE;
        Effect effect;
        bool inactive = false;
        EventListener(const Effect &effect_) : effect(effect_) {}
        EventListener(Id entityId_, const Effect &effect_) : entityId(entityId_), effect(effect_) {}
    };

    using EventListeners = std::vector<EventListener>;
    using EventRegistry = EventListeners[int(EventType::Count)];
    using EventList = std::vector<std::pair<EventType, EventListener>>;

    std::string ToString(EventRegistry &registry) {
        std::stringstream ss;
        for(int i = 0; i < int(EventType::Count); i++) {
            auto& listeners = registry[i];
            if(listeners.size() > 0) ss << "EventType " << i << "; ";
            for(auto& listener : listeners) {
                ss << "[ id: " << listener.entityId << "; inactive: " << listener.inactive
                    << "; effect: [" << listener.effect.toString() << " ] ] ";
            }
        }
        return ss.str();
    }

}
