#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::registerEventsFromList(EventList& eventList, Id entityId) {
        for(auto& [type, listener] : eventList) {
            listener.effect.sourceEntityId = entityId;
            registerEvent(int (type), EventListener(entityId, listener.effect));
        }
    }

    template<typename T>
    void CombatState::registerEventsFromEntity(T &t, Id entityId) {
        registerEventsFromList(t.eventList, entityId);
    }
    
    void CombatState::registerEventsFromEntity(Id entityId) {
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

    void CombatState::unregisterEventsFromEntity(Id entityId) {
        auto& refList = ecs.cRefs[entityId].list;
        for(auto& entry : refList) {
            eventRegistry[entry.eventType][entry.eventIndex].inactive = true;
        }
    }

    void CombatState::registerEvent(int eventType, const EventListener &eventListener) {
        if(eventListener.entityId >= 0) {
            assert(ecs.cRefs.size() > eventListener.entityId);
            auto& refList = ecs.cRefs[eventListener.entityId].list;
            refList.push_back(EventRef(eventType, eventRegistry[eventType].size()));
        }
        eventRegistry[eventType].push_back(eventListener);
    }
    
    void CombatState::triggerEvent(EventType eventType) {
        auto& listeners = eventRegistry[int(eventType)];
        for(auto& listener : listeners) {
            if(listener.inactive) continue;
            auto id = listener.entityId;
            if(!evaluateConditions(listener.effect)) continue;
            auto& e = listener.effect;
            putEffectOntoStack(Effect(e.effectType, e.params, e.sourceEntityId));
        }
    }

    void CombatState::triggerInterceptor(EventType eventType, const InterceptorContext &context, int &value) {
        auto& listeners = eventRegistry[int(eventType)];
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
