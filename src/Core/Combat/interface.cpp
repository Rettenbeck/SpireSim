#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    Actions& CombatState::getActions() {
        return actions;
    }

    bool CombatState::isCombatOver() {
        return combatOver;
    }

    bool CombatState::isCombatVictorious() {
        return (variables.enemies == 0 ? true : false);
    }

    int CombatState::getPlayerHealth() {
        return ecs.getPlayer().data.hp;
    }

    int CombatState::getPlayerBlock() {
        return ecs.getPlayer().data.block;
    }

    int CombatState::getTotalEnemyHp() {
        int sum = 0;
        for(auto enemyId : ecs.enemyEntityIds) {
            sum += ecs.getEnemy(enemyId).data.hp;
        }
        return sum;
    }

}
