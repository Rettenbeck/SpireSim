#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    EnemyMoveId CombatState::getNextMove(EnemyMoveId move) {
        auto& nextMoves = NextEnemyMoveList[move];
        assert(!nextMoves.empty());
        if(nextMoves.size() == 1) return nextMoves[0];
        return nextMoves[getRandomNumber(nextMoves.size() - 1)];
    }

    void CombatState::advanceMove(EnemyMoveId &move) {
        move = getNextMove(move);
    }

    void CombatState::resolveMove(Id enemyEntityId, Enemy &enemy, EnemyMove &move) {
        if(move.attacks > 0) {
            for(int i = 0; i < move.attacks; i++)
                dealDamage(enemyEntityId, enemy.data, ecs.playerEntityId, ecs.getPlayer().data, move.damage);
            move.vigor = 0;
        }
        if(move.block           > 0) { gainBlock(enemy.data, move.block); }
        if(move.strengthGain    > 0) { gainStrength(enemy.data, move.strengthGain); }
        if(move.vigor           > 0) { gainVigor(enemy.data, move.vigor); }
        if(move.applyVulnerable > 0) { applyVulnerableToPlayer(move.applyVulnerable); }
        if(move.applyWeak       > 0) { applyWeakToPlayer(move.applyWeak); }
        if(move.applyFrail      > 0) { applyFrailToPlayer(move.applyFrail); }

        for(auto& buff : move.buffsToGain) {
            applyBuff(buff, enemyEntityId, enemyEntityId);
        }
        for(auto& debuff : move.debuffsToApply) {
            applyBuff(debuff, enemyEntityId, ecs.playerEntityId);
        }
    }

    void CombatState::executeMove(Id enemyEntityId) {
        auto& enemy = ecs.getEnemy(enemyEntityId);
        if(enemy.currentMove == EnemyMoveId::None) {
            assert(enemy.initialMoveCount < enemy.initialMoves.size());
            enemy.currentMove = enemy.initialMoves[enemy.initialMoveCount];
            enemy.initialMoveCount++;
        } else {
            advanceMove(enemy.currentMove);
        }
        assert(enemy.currentMove != EnemyMoveId::None);
        auto& move = movePool.retrieve(enemy.currentMove);
        resolveMove(enemyEntityId, enemy, move);
    }

    void CombatState::executeMoves() {
        for(auto& id : ecs.enemyEntityIds) executeMove(id);
    }

}
