#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    void Combat::registerEventsFromList(EventList& eventList, Id entityId) {
        for(auto& [type, listener] : eventList) {
            listener.effect.sourceEntityId = entityId;
            registerEvent(int (type), EventListener(entityId, listener.effect));
        }
    }

    template<typename T>
    void Combat::registerEventsFromEntity(T &t, Id entityId) {
        registerEventsFromList(t.eventList, entityId);
    }
    
    void Combat::registerEventsFromEntity(Id entityId) {
        if(ecs.isCard(entityId)) {
            auto& cardTemplate = cardPool.retrieve(ecs.getCard(entityId).cardId);
            registerEventsFromEntity(cardTemplate, entityId);
            return;
        }
        if(ecs.isBuff(entityId)) {
            auto& buffTemplate = buffPool.retrieve(ecs.getBuff(entityId).buffId);
            registerEventsFromEntity(buffTemplate, entityId);
            return;
        }
        if(ecs.isEnemy(entityId)) {
            registerEventsFromEntity(ecs.getEnemy(entityId), entityId);
            return;
        }
        //if(ecs.isPlayer(entityId)) registerEventsFromList(ecs.getPlayer(        ), entityId);
    }

    void Combat::unregisterEventsFromEntity(Id entityId) {
        auto& refList = ecs.cRefs[entityId].list;
        for(auto& entry : refList) {
            state.eventRegistry[entry.eventType][entry.eventIndex].inactive = true;
        }
    }

    void Combat::registerEvent(int eventType, const EventListener &eventListener) {
        if(eventListener.entityId >= 0) {
            assert(ecs.cRefs.size() > eventListener.entityId);
            auto& refList = ecs.cRefs[eventListener.entityId].list;
            refList.push_back(EventRef(eventType, state.eventRegistry[eventType].size()));
        }
        state.eventRegistry[eventType].push_back(eventListener);
    }
    
    void Combat::triggerEvent(EventType eventType) {
        auto& listeners = state.eventRegistry[int(eventType)];
        for(auto& listener : listeners) {
            if(listener.inactive) continue;
            auto id = listener.entityId;
            if(!evaluateConditions(listener.effect)) continue;
            auto& e = listener.effect;
            putEffectOntoStack(Effect(e.effectType, e.params, e.sourceEntityId));
        }
    }

    void Combat::triggerInterceptor(EventType eventType, const InterceptorContext &context, int &value) {
        auto& listeners = state.eventRegistry[int(eventType)];
        for(auto& listener : listeners) {
            if(listener.inactive) continue;
            auto id = listener.entityId;
            if(!evaluateConditions(listener.effect)) continue;
            auto& e = listener.effect;
            Effect effect(e.effectType, e.params, e.sourceEntityId);
            resolveInterceptor(effect, context, value);
        }
    }

}
