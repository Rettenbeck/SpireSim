#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::endFight(bool won) {
        //std::cout << "KAMPF VORBEI! Gewonnen? " << won << "\n";
        waitingForActionOnStack = false;
        waitingForAction = false;
        stopStack = true;
        combatOver = true;
    }

    void CombatState::checkForFightEnd() {
        if(variables.enemies == 0) endFight(true);
        if(ecs.getPlayer().playerId == PlayerId::None) endFight(false);
    }

    void CombatState::entityDies(Id entityId) {
        if(ecs.cEnemies[entityId].enemyId != EnemyId ::None) {
            if(!ecs.cEnemies[entityId].isMinion) variables.enemies--;
            ecs.cEnemies[entityId].enemyId  = EnemyId ::None;
            ecs.buildEnemyEntityIds();
            checkForFightEnd();
        }
        if(entityId == ecs.playerEntityId) {
            ecs.getPlayer().playerId = PlayerId::None;
            endFight(false);
        }
        ecs.cCards[entityId].cardId = CardId::None;
    }

    inline int CombatState::calculateDamage(Id sourceEntityId, CharacterData &sourceData,
                                            Id targetEntityId, CharacterData &targetData,
                                            int damage)
    {
        float wFactor = (sourceData.weak       > 0) ? sourceData.weakFactor       : 1.0f;
        float vFactor = (targetData.vulnerable > 0) ? targetData.vulnerableFactor : 1.0f;
        return (int) (wFactor * vFactor * ((float) (damage + sourceData.strength + sourceData.tmpStrength + sourceData.vigor)));
    }

    void CombatState::dealDamage(   Id sourceEntityId, CharacterData &sourceData,
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
            if(targetData.hp <= 0) entityDies(targetEntityId);
        }
    }

    inline void CombatState::dealDamageToEnemy(Id sourceEntityId, CharacterData &sourceData, Id targetEntityId, int damage) {
        auto& enemy = ecs.getEnemy(targetEntityId);
        if(enemy.enemyId == EnemyId::None) {
            // fizzle...
            return;
        }
        dealDamage(sourceEntityId, sourceData, targetEntityId, enemy.data, damage);
    }



    void CombatState::applyBuff(Buff &buff, Id sourceEntityId, Id targetEntityId) {
        buff.createdBy = sourceEntityId;
        auto id = ecs.addObject(buff, targetEntityId);
        registerEventsFromEntity(buffPool.retrieve(buff.buffId), id);
    }

    

    inline void CombatState::gainBlock(CharacterData &data, int block) {
        data.block += block;
    }

    inline void CombatState::gainBlockPlayer(int block) {
        gainBlock(ecs.getPlayer().data, block);
    }

    inline void CombatState::gainBlockEnemy(Enemy &enemy, int block) {
        gainBlock(enemy.data, block);
    }

    inline void CombatState::gainBlockEnemy(Id &enemyEntityId, int block) {
        gainBlock(ecs.getEnemy(enemyEntityId).data, block);
    }

    void CombatState::gainBlock(Id entityId, int block) {
        if(entityId == ecs.playerEntityId) {
            gainBlockPlayer(block);
        } else {
            gainBlockEnemy(entityId, block);
        }
    }



    inline void CombatState::gainStrength(CharacterData &targetData, int value) {
        targetData.strength += value;
    }

    inline void CombatState::gainStrengthEnemy(Id targetEntityId, int value) {
        gainStrength(ecs.getEnemy(targetEntityId).data, value);
    }

    inline void CombatState::gainStrengthPlayer(int value) {
        gainStrength(ecs.getPlayer().data, value);
    }



    inline void CombatState::gainDexterity(CharacterData &targetData, int value) {
        targetData.dex += value;
    }

    inline void CombatState::gainDexterityPlayer(int value) {
        gainDexterity(ecs.getPlayer().data, value);
    }



    inline void CombatState::applyVulnerable(CharacterData &targetData, int value) {
        targetData.vulnerable += value;
    }

    inline void CombatState::applyVulnerableToEnemy(Id targetEntityId, int value) {
        applyVulnerable(ecs.getEnemy(targetEntityId).data, value);
    }

    inline void CombatState::applyVulnerableToPlayer(int value) {
        applyVulnerable(ecs.getPlayer().data, value);
    }
    


    inline void CombatState::applyWeak(CharacterData &targetData, int value) {
        targetData.weak += value;
    }

    inline void CombatState::applyWeakToEnemy(Id targetEntityId, int value) {
        applyWeak(ecs.getEnemy(targetEntityId).data, value);
    }

    inline void CombatState::applyWeakToPlayer(int value) {
        applyWeak(ecs.getPlayer().data, value);
    }
    


    inline void CombatState::applyFrail(CharacterData &targetData, int value) {
        targetData.frail += value;
    }

    inline void CombatState::applyFrailToPlayer(int value) {
        applyFrail(ecs.getPlayer().data, value);
    }
    


    inline void CombatState::gainVigor(CharacterData &targetData, int value) {
        targetData.vigor += value;
    }

    inline void CombatState::gainVigorEnemy(Id targetEntityId, int value) {
        gainVigor(ecs.getEnemy(targetEntityId).data, value);
    }

    inline void CombatState::gainVigorPlayer(int value) {
        gainVigor(ecs.getPlayer().data, value);
    }



    inline void CombatState::gainTempStrength(CharacterData &targetData, int value) {
        targetData.tmpStrength += value;
    }

    inline void CombatState::gainTempStrengthPlayer(int value) {
        gainTempStrength(ecs.getPlayer().data, value);
    }

    inline void CombatState::gainTempDexterity(CharacterData &targetData, int value) {
        targetData.tmpDex += value;
    }

    inline void CombatState::gainTempDexterityPlayer(int value) {
        gainTempDexterity(ecs.getPlayer().data, value);
    }

}
