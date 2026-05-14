#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::resolveIntent(Id enemyEntityId, Enemy &enemy, EnemyMove &move) {
        if(move.attacks > 0) {
            for(int i = 0; i < move.attacks; i++)
                dealDamage(enemyEntityId, enemy.data, ecs.playerEntityId, ecs.getPlayer().data, move.damage);
            move.vigor = 0;
        }
        if(move.block > 0) { gainBlock(enemy.data, move.block); }
        if(move.strengthGain > 0) { gainStrength(enemy.data, move.strengthGain); }
        if(move.vigor > 0) { gainVigor(enemy.data, move.vigor); }
        if(move.applyVulnerable > 0) { applyVulnerableToPlayer(move.applyVulnerable); }
        if(move.applyWeak > 0) { applyWeakToPlayer(move.applyWeak); }
        if(move.applyFrail > 0) { applyFrailToPlayer(move.applyFrail); }

    }

    void CombatState::executeIntent(Id enemyEntityId) {
        auto& enemy = ecs.getEnemy(enemyEntityId);
        assert(enemy.currentIntentStrategy < enemy.intentStrategies.size());
        auto& move = movePool.retrieve(enemy.intentStrategies[enemy.currentIntentStrategy].generateNextIntent());
        resolveIntent(enemyEntityId, enemy, move);
    }

    void CombatState::executeIntents() {
        for(auto& id : ecs.enemyEntityIds) executeIntent(id);
    }

}
