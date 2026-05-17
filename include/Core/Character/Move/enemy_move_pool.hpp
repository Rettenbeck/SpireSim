#pragma once

#include <Core/Buff/buff_pool.hpp>
#include <Core/Character/Move/enemy_move.hpp>


namespace SpireSim {

    struct EnemyMovePool {
        BuffPool &buffPool;
        int isDifficult;
        EnemyMoves enemyMoves;
        
        EnemyMovePool(BuffPool &buffPool_, int isDifficult_ = 0) : buffPool(buffPool_), isDifficult(isDifficult_) {
            fillNextEnemyMoveList();
            createEnemyMovePool();
        }
        
        void createBlankArray(int index) {
            while(index >= enemyMoves.size()) {
                enemyMoves.push_back(EnemyMove(buffPool));
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

        
        void createEnemyMovePool() {
            createBlankArray(int(EnemyMoveId::Count));
            
            retrieveForCreation(EnemyMoveId::Shrinker).addDebuff(BuffId::Shrink);
            retrieveForCreation(EnemyMoveId::Chomp).addAttack( 7 + 1 * isDifficult);
            retrieveForCreation(EnemyMoveId::Stomp).addAttack(13 + 1 * isDifficult);

            retrieveForCreation(EnemyMoveId::AcidGoop).addAttack(4 + 2 * isDifficult);
            retrieveForCreation(EnemyMoveId::Inhale).gainStrength(7);

            retrieveForCreation(EnemyMoveId::Butt ).addAttack(12 + 1 * isDifficult);
            retrieveForCreation(EnemyMoveId::Slice).addAttack(6).addBlock(5);
            retrieveForCreation(EnemyMoveId::Hiss ).gainStrength(2);
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
