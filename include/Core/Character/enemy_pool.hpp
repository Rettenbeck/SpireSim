#pragma once

#include <Core/Character/enemy.hpp>
#include <Core/Character/Move/enemy_move_pool.hpp>


namespace SpireSim {

    struct EnemyPool {
        int isDifficult;
        Enemies enemies;
        
        EnemyPool(EnemyMovePool &enemyMovePool, int isDifficult_ = 0) : isDifficult(isDifficult_) {
            createEnemyPool(enemyMovePool);
        }
        
        void createBlankArray(int index) {
            while(index >= enemies.size()) {
                enemies.push_back(Enemy());
            }
        }
        // 
        Enemy& retrieveForCreation(EnemyId enemyId) {
            int index = int(enemyId);
            createBlankArray(index);
            assert(enemies.size() > index);
            enemies[index].enemyId = enemyId;
            return enemies[index];
        }
        
        Enemy& retrieveForCreation(EnemyId enemyId, EnemyMoveId initialMove, int maxHp, int potentialAdditionalMaxHp = 0) {
            auto& enemy = retrieveForCreation(enemyId);
            enemy.initialMoves.push_back(initialMove);
            enemy.setMaxHp(maxHp, potentialAdditionalMaxHp);
            return enemy;
        }
        
        inline Enemy& retrieve(EnemyId enemyId) {
            int index = int(enemyId);
            assert(index < enemies.size());
            return enemies[index];
        }
        
        void createEnemyPool(EnemyMovePool &enemyMovePool) {
            createBlankArray(int(EnemyId::Count));
            
            retrieveForCreation(EnemyId::ShrinkerBeetle, EnemyMoveId::Shrinker, 38 + 2 * isDifficult, 2);
            retrieveForCreation(EnemyId::FuzzyWormCrawler, EnemyMoveId::AcidGoop, 55 + 2 * isDifficult, 2);
            retrieveForCreation(EnemyId::Nibbit, EnemyMoveId::Butt, 42 + 2 * isDifficult, 4);
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "EnemyPool: { ";
            for(auto enemy : enemies) ss << "[ " << enemy.toString() << "] ";
            ss << "};";
            return ss.str();
        }
    };

}
