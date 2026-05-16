#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    void Combat::endFight(bool won) {
        state.waitingForActionOnStack = false;
        state.waitingForAction = false;
        state.stopStack = true;
        state.combatOver = true;
    }

    void Combat::checkForFightEnd() {
        if(variables.enemies == 0) endFight(true);
        if(ecs.getPlayer().playerId == PlayerId::None) endFight(false);
    }

    void Combat::entityDies(Id entityId) {
        if(ecs.cEnemies[entityId].enemyId != EnemyId ::None) {
            if(!ecs.cEnemies[entityId].isMinion) variables.enemies--;
            ecs.cEnemies[entityId].enemyId = EnemyId ::None;
            ecs.buildEnemyEntityIds();
            checkForFightEnd();

            for(auto& entity : ecs.entities) {
                if(!ecs.isBuff(entity.id)) continue;
                auto& buff = ecs.getBuff(entity.id);
                if(buff.createdBy == entityId && buffPool.retrieve(buff.buffId).dependentOnCreator) {
                    buff.buffId = BuffId::None;
                    unregisterEventsFromEntity(entity.id);
                }
            }
            unregisterEventsFromEntity(entityId);
        }
        if(entityId == ecs.playerEntityId) {
            ecs.getPlayer().playerId = PlayerId::None;
            endFight(false);
        }
        ecs.cCards[entityId].cardId = CardId::None;
    }

    inline void Combat::onDamageDealt(  Id sourceEntityId, CharacterData &sourceData,
                                        Id targetEntityId, CharacterData &targetData,
                                        int damage)
    {
        if(targetEntityId == ecs.playerEntityId) {
            if(variables.hasTakenDamage == 0) triggerEvent(EventType::OnFirstDamageTaken);
            variables.hasTakenDamage = 1;
        }
    }

    inline int Combat::calculateDamage( Id sourceEntityId, CharacterData &sourceData,
                                        Id targetEntityId, CharacterData &targetData,
                                        int damage)
    {
        float wFactor = (sourceData.weak       > 0) ? sourceData.weakFactor       : 1.0f;
        float vFactor = (targetData.vulnerable > 0) ? targetData.vulnerableFactor : 1.0f;
        return (int) (wFactor * vFactor * ((float) (damage + sourceData.strength + sourceData.tmpStrength + sourceData.vigor)));
    }

    void Combat::dealDamage(Id sourceEntityId, CharacterData &sourceData,
                            Id targetEntityId, CharacterData &targetData,
                            int damage)
    {
        int damageCalculated = calculateDamage(sourceEntityId, sourceData, targetEntityId, targetData, damage);
        triggerInterceptor(EventType::OnDealDamageForInterception, InterceptorContext(sourceEntityId), damageCalculated);
        if(targetData.block >= damageCalculated) {
            targetData.block -= damageCalculated;
        } else {
            damageCalculated -= targetData.block;
            targetData.block = 0;
            targetData.hp -= damageCalculated;
            if(damageCalculated > 0) onDamageDealt(sourceEntityId, sourceData, targetEntityId, targetData, damageCalculated);
            if(targetData.hp <= 0) entityDies(targetEntityId);
        }
    }

    inline void Combat::dealDamageToEnemy(Id sourceEntityId, CharacterData &sourceData, Id targetEntityId, int damage) {
        auto& enemy = ecs.getEnemy(targetEntityId);
        if(enemy.enemyId == EnemyId::None) {
            // fizzle...
            return;
        }
        dealDamage(sourceEntityId, sourceData, targetEntityId, enemy.data, damage);
    }



    void Combat::applyBuff(Buff &buff, Id sourceEntityId, Id targetEntityId) {
        buff.createdBy = sourceEntityId;
        auto id = ecs.addObject(buff, targetEntityId);
        registerEventsFromEntity(buffPool.retrieve(buff.buffId), id);
    }

    

    inline void Combat::gainBlock(CharacterData &data, int block) {
        data.block += block;
    }

    inline void Combat::gainBlockPlayer(int block) {
        gainBlock(ecs.getPlayer().data, block);
    }

    inline void Combat::gainBlockEnemy(Enemy &enemy, int block) {
        gainBlock(enemy.data, block);
    }

    inline void Combat::gainBlockEnemy(Id &enemyEntityId, int block) {
        gainBlock(ecs.getEnemy(enemyEntityId).data, block);
    }

    void Combat::gainBlock(Id entityId, int block) {
        if(entityId == ecs.playerEntityId) {
            gainBlockPlayer(block);
        } else {
            gainBlockEnemy(entityId, block);
        }
    }



    inline void Combat::gainStrength(CharacterData &targetData, int value) {
        targetData.strength += value;
    }

    inline void Combat::gainStrengthEnemy(Id targetEntityId, int value) {
        gainStrength(ecs.getEnemy(targetEntityId).data, value);
    }

    inline void Combat::gainStrengthPlayer(int value) {
        gainStrength(ecs.getPlayer().data, value);
    }



    inline void Combat::gainDexterity(CharacterData &targetData, int value) {
        targetData.dex += value;
    }

    inline void Combat::gainDexterityPlayer(int value) {
        gainDexterity(ecs.getPlayer().data, value);
    }



    inline void Combat::applyVulnerable(CharacterData &targetData, int value) {
        targetData.vulnerable += value;
    }

    inline void Combat::applyVulnerableToEnemy(Id targetEntityId, int value) {
        applyVulnerable(ecs.getEnemy(targetEntityId).data, value);
    }

    inline void Combat::applyVulnerableToPlayer(int value) {
        applyVulnerable(ecs.getPlayer().data, value);
    }
    


    inline void Combat::applyWeak(CharacterData &targetData, int value) {
        targetData.weak += value;
    }

    inline void Combat::applyWeakToEnemy(Id targetEntityId, int value) {
        applyWeak(ecs.getEnemy(targetEntityId).data, value);
    }

    inline void Combat::applyWeakToPlayer(int value) {
        applyWeak(ecs.getPlayer().data, value);
    }
    


    inline void Combat::applyFrail(CharacterData &targetData, int value) {
        targetData.frail += value;
    }

    inline void Combat::applyFrailToPlayer(int value) {
        applyFrail(ecs.getPlayer().data, value);
    }
    


    inline void Combat::gainVigor(CharacterData &targetData, int value) {
        targetData.vigor += value;
    }

    inline void Combat::gainVigorEnemy(Id targetEntityId, int value) {
        gainVigor(ecs.getEnemy(targetEntityId).data, value);
    }

    inline void Combat::gainVigorPlayer(int value) {
        gainVigor(ecs.getPlayer().data, value);
    }



    inline void Combat::gainTempStrength(CharacterData &targetData, int value) {
        targetData.tmpStrength += value;
    }

    inline void Combat::gainTempStrengthPlayer(int value) {
        gainTempStrength(ecs.getPlayer().data, value);
    }

    inline void Combat::gainTempDexterity(CharacterData &targetData, int value) {
        targetData.tmpDex += value;
    }

    inline void Combat::gainTempDexterityPlayer(int value) {
        gainTempDexterity(ecs.getPlayer().data, value);
    }

}
