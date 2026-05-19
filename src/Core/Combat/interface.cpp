#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    const Actions& Combat::getActions() const {
        return state.actions;
    }

    bool Combat::isCombatOver() {
        return state.combatOver;
    }

    bool Combat::isCombatVictorious() {
        return (variables.enemies == 0 ? true : false);
    }

    int Combat::getPlayerMaxHealth() {
        return ecs.getPlayer().data.maxHp;
    }

    int Combat::getPlayerHealth() {
        return ecs.getPlayer().data.hp;
    }

    int Combat::getPlayerBlock() {
        return ecs.getPlayer().data.block;
    }

    int Combat::getTotalEnemyHp() {
        int sum = 0;
        for(auto enemyId : ecs.enemyEntityIds) {
            sum += ecs.getEnemy(enemyId).data.hp;
        }
        return sum;
    }

    int Combat::getTotalEnemyMaxHp() {
        int sum = 0;
        for(auto enemyId : ecs.enemyEntityIds) {
            sum += ecs.getEnemy(enemyId).data.maxHp;
        }
        return sum;
    }

    int Combat::getHpLoss() {
        return variables.initialHp - ecs.getPlayer().data.hp;
    }

    void Combat::setDifficulty(bool enemiesMoreHp, bool enemiesDeadlier) {
        variables.enemiesMoreHp = enemiesMoreHp;
        variables.enemiesDeadlier = enemiesDeadlier;
    }

    CardId Combat::getCardIdFromEntityId(Id cardEntityId) {
        return ecs.getCard(cardEntityId).cardId;
    }

}
