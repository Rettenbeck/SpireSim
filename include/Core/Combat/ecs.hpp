#pragma once

#include <Core/Combat/entity.hpp>
#include <Core/Card/card.hpp>
#include <Core/Character/player.hpp>
#include <Core/Character/enemy.hpp>
#include <Core/Event/event_refs.hpp>


namespace SpireSim {

    struct ECS {
        Entities entities;

        Cards cCards;
        Players cPlayers;
        Enemies cEnemies;
        EventRefs cRefs;

        Id playerEntityId = ENTITY_NONE;
        Ids enemyEntityIds;

        template<typename T>
        void fillVector(std::vector<T> &vec, int size) {
            while(vec.size() <= size) vec.push_back(T());
        }

        Id createEntity(Id ownerId = ENTITY_NONE) {
            auto id = entities.size();
            entities.push_back(Entity(id, ownerId));
            fillVector(cCards  , id);
            fillVector(cPlayers, id);
            fillVector(cEnemies, id);
            fillVector(cRefs   , id);
            return id;
        }
        
        template<typename T>
        inline Id addObject(std::vector<T> &vec, const T &t, Id ownerId = ENTITY_NONE) {
            auto id = createEntity(ownerId);
            assert(vec.size() > id);
            vec[id] = t;
            return id;
        }
        
        void buildEnemyEntityIds() {
            enemyEntityIds.clear();
            for(auto& entity : entities) {
                auto& enemy = cEnemies[entity.id];
                if(enemy.enemyId == EnemyId::None) continue;
                enemyEntityIds.push_back(entity.id);
            }
        }

        inline Id addObject(const Card   &card  ) { return addObject(cCards  , card  ); }
        inline Id addObject(const Player &player) { return addObject(cPlayers, player); }
        inline Id addObject(const Enemy  &enemy ) { return addObject(cEnemies, enemy ); }

        void registerPlayer(const Player &player_) {
            playerEntityId = addObject(player_);
        }

        void registerEnemies(const Enemies &enemies_) {
            for(auto& enemy : enemies_) addObject(enemy);
            buildEnemyEntityIds();
        }
        
        inline Card& getCard(Id entityId) {
            assert(entities.size() > entityId);
            assert(cCards  .size() > entityId);
            auto& card = cCards[entityId];
            assert(card.cardId != CardId::None);
            return cCards[entityId];
        }

        inline Player& getPlayer() {
            assert(entities.size() > playerEntityId);
            assert(cPlayers.size() > playerEntityId);
            return cPlayers[playerEntityId];
        }

        inline Enemy& getEnemy(Id entityId) {
            assert(entities.size() > entityId);
            assert(cEnemies.size() > entityId);
            return cEnemies[entityId];
        }

        inline bool isCard  (Id entityId) { return cCards  [entityId].cardId   != CardId  ::None; }
        inline bool isPlayer(Id entityId) { return cPlayers[entityId].playerId != PlayerId::None; }
        inline bool isEnemy (Id entityId) { return cEnemies[entityId].enemyId  != EnemyId ::None; }

        inline isValidEntity(Id entityId) {
            return (isCard(entityId) || isPlayer(entityId) || isEnemy(entityId));
        }

        template<typename T>
        std::string vectorToString(std::string description, std::vector<T> &vec) {
            std::stringstream ss;
            ss << description << ": { ";
            for(auto e : vec) ss << "\n" << "[ " << e.toString() << "] ";
            ss << "};\n";
            return ss.str();
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "\n";
            ss << "  Entities: { ";
            for(auto e : entities) {
                ss << "\n[ " << e.toString() << "] \n";
                auto& card   = cCards  [e.id];
                auto& player = cPlayers[e.id];
                auto& enemy  = cEnemies[e.id];
                auto& refs   = cRefs   [e.id];

                if(card  .cardId   != CardId  ::None) ss << "[ " << card  .toString() << "] \n";
                if(player.playerId != PlayerId::None) ss << "[ " << player.toString() << "] \n";
                if(enemy .enemyId  != EnemyId ::None) ss << "[ " << enemy .toString() << "] \n";
                if(refs.list.size() > 0) ss << "[ " << refs.toString() << "] \n";
            }
            ss << "} \n\n";
            
            ss << "playerEntityId: " << playerEntityId << "; enemyEntityIds: { ";
            for(auto i : enemyEntityIds) ss << i << " ";
            ss << "}\n";
        
            return ss.str();
        }
    };

}
