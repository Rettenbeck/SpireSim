#pragma once

#include <Core/Combat/entity.hpp>
#include <Core/Relic/relic.hpp>
#include <Core/Potion/potion.hpp>
#include <Core/Card/card.hpp>
#include <Core/Character/player.hpp>
#include <Core/Character/enemy.hpp>
#include <Core/Buff/buff.hpp>
#include <Core/Event/event_refs.hpp>


namespace SpireSim {

    struct ECS {
        Entities entities;

        Relics      cRelics;
        Potions     cPotions;
        Cards       cCards;
        Players     cPlayers;
        Enemies     cEnemies;
        Buffs       cBuffs;
        EventRefs   cRefs;

        Id playerEntityId = ENTITY_NONE;
        Ids enemyEntityIds;

        template<typename T>
        void fillVector(std::vector<T> &vec, int size) {
            while(vec.size() <= size) vec.push_back(T());
        }

        Id createEntity(Id ownerId = ENTITY_NONE) {
            auto id = entities.size();
            entities.push_back(Entity(id, ownerId));
            fillVector(cRelics , id);
            fillVector(cPotions, id);
            fillVector(cCards  , id);
            fillVector(cPlayers, id);
            fillVector(cEnemies, id);
            fillVector(cBuffs  , id);
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

        inline Id getParent(Id entityId) {
            assert(entityId < entities.size());
            return entities[entityId].ownerId;
        }

        inline Id addObject(const Relic  &relic ) { return addObject(cRelics , relic ); }
        inline Id addObject(const Potion &potion) { return addObject(cPotions, potion); }
        inline Id addObject(const Card   &card  ) { return addObject(cCards  , card  ); }
        inline Id addObject(const Player &player) { return addObject(cPlayers, player); }
        inline Id addObject(const Enemy  &enemy ) { return addObject(cEnemies, enemy ); }
        inline Id addObject(const Buff &buff, Id ownerId) { return addObject(cBuffs, buff, ownerId); }

        void registerPlayer(const Player &player_) {
            playerEntityId = addObject(player_);
        }

        void registerEnemies(const Enemies &enemies_) {
            for(auto& enemy : enemies_) addObject(enemy);
            buildEnemyEntityIds();
        }
        
        inline CharacterData& getCharacterData(Id entityId) {
            if(isPlayer(entityId)) return getPlayer().data;
            return getEnemy(entityId).data;
        }

        inline Relic& getRelic(Id entityId) {
            assert(entities.size() > entityId);
            assert(cRelics .size() > entityId);
            auto& relic = cRelics[entityId];
            assert(relic.relicId != RelicId::None);
            return cRelics[entityId];
        }

        inline Potion& getPotion(Id entityId) {
            assert(entities.size() > entityId);
            assert(cPotions.size() > entityId);
            auto& potion = cPotions[entityId];
            assert(potion.potionId != PotionId::None);
            return cPotions[entityId];
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

        inline Buff& getBuff(Id entityId) {
            assert(entities.size() > entityId);
            assert(cBuffs  .size() > entityId);
            return cBuffs[entityId];
        }

        inline bool isRelic (Id entityId) { return cRelics [entityId].relicId  != RelicId ::None; }
        inline bool isPotion(Id entityId) { return cPotions[entityId].potionId != PotionId::None; }
        inline bool isCard  (Id entityId) { return cCards  [entityId].cardId   != CardId  ::None; }
        inline bool isPlayer(Id entityId) { return cPlayers[entityId].playerId != PlayerId::None; }
        inline bool isEnemy (Id entityId) { return cEnemies[entityId].enemyId  != EnemyId ::None; }
        inline bool isBuff  (Id entityId) { return cBuffs  [entityId].buffId   != BuffId  ::None; }

        inline isValidEntity(Id entityId) {
            return (isRelic (entityId) ||
                    isPotion(entityId) ||
                    isCard  (entityId) ||
                    isPlayer(entityId) ||
                    isEnemy (entityId) ||
                    isBuff  (entityId)  );
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
                auto& relic  = cRelics [e.id];
                auto& potion = cPotions[e.id];
                auto& card   = cCards  [e.id];
                auto& player = cPlayers[e.id];
                auto& enemy  = cEnemies[e.id];
                auto& buff   = cBuffs  [e.id];
                auto& refs   = cRefs   [e.id];

                if(relic .relicId  != RelicId ::None) ss << "[ " << relic .toString() << "] \n";
                if(potion.potionId != PotionId::None) ss << "[ " << potion.toString() << "] \n";
                if(card  .cardId   != CardId  ::None) ss << "[ " << card  .toString() << "] \n";
                if(player.playerId != PlayerId::None) ss << "[ " << player.toString() << "] \n";
                if(enemy .enemyId  != EnemyId ::None) ss << "[ " << enemy .toString() << "] \n";
                if(buff  .buffId   != BuffId  ::None) ss << "[ " << buff  .toString() << "] \n";
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
