#pragma once

#include <Core/Buff/buff_pool.hpp>
#include <Core/Character/Intent/enemy_move.hpp>


namespace SpireSim {

    struct EnemyMovePool {
        int isDifficult;
        EnemyMoves enemyMoves;
        
        EnemyMovePool(BuffPool &buffPool, int isDifficult_ = 0) : isDifficult(isDifficult_) {
            createEnemyMovePool(buffPool);
        }
        
        void createBlankArray(int index) {
            while(index >= enemyMoves.size()) {
                enemyMoves.push_back(EnemyMove());
            }
        }
        
        EnemyMove& retrieveForCreation(EnemyMoveId enemyMoveId) {
            int index = int(enemyMoveId);
            createBlankArray(index);
            enemyMoves[index].enemyMoveId = enemyMoveId;
            return enemyMoves[index];
        }
        
        EnemyMove& retrieve(EnemyMoveId enemyMoveId) {
            int index = int(enemyMoveId);
            assert(index < enemyMoves.size());
            return enemyMoves[index];
        }

        
        void createEnemyMovePool(BuffPool &buffPool) {
            createBlankArray(int(EnemyMoveId::Count));
            
            auto& moveShrinker = retrieveForCreation(EnemyMoveId::Shrinker);
            auto& moveChomp = retrieveForCreation(EnemyMoveId::Chomp);
            auto& moveStomp = retrieveForCreation(EnemyMoveId::Stomp);
            
            auto& moveAcidGoop = retrieveForCreation(EnemyMoveId::AcidGoop);
            auto& moveInhale = retrieveForCreation(EnemyMoveId::Inhale);
            
            moveShrinker.debuffsToApply.push_back(buffPool.createBuffFromTemplate(BuffId::Shrink));
            moveChomp.addAttack( 7 + 1 * isDifficult);
            moveStomp.addAttack(13 + 1 * isDifficult);
            
            moveAcidGoop.addAttack(4 + 2 * isDifficult);
            moveInhale.strengthGain = 7;
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "EnemyMoves: { ";
            for(auto& move : enemyMoves) ss << "[ " << move.toString() << "] ";
            ss << "}";
            return ss.str();
        }
    };

}
