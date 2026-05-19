#pragma once

#include <Core/Character/enemy.hpp>
#include <Core/Character/Move/enemy_move_pool.hpp>


namespace SpireSim {

    struct EnemyPool {
        Enemies enemies;
        
        EnemyPool(EnemyMovePool &enemyMovePool) {
            createEnemyPool(enemyMovePool);
        }
        
        void createBlankArray(int index) {
            while(index >= enemies.size()) {
                enemies.push_back(Enemy());
            }
        }
        
        Enemy& retrieveForCreation(EnemyId enemyId) {
            int index = int(enemyId);
            createBlankArray(index);
            assert(enemies.size() > index);
            enemies[index].enemyId = enemyId;
            return enemies[index];
        }
        
        Enemy& retrieveForCreation( EnemyId enemyId,
                                    EnemyMoveId initialMove,
                                    int maxHpLowerBound, int maxHpUpperBound,
                                    int maxHpLowerBoundHard, int maxHpUpperBoundHard
                                )
        {
            auto& enemy = retrieveForCreation(enemyId);
            enemy.initialMoves.push_back(initialMove);
            enemy.setMaxHp(maxHpLowerBound, maxHpUpperBound, maxHpLowerBoundHard, maxHpUpperBoundHard);
            return enemy;
        }
        
        inline Enemy& retrieve(EnemyId enemyId) {
            int index = int(enemyId);
            assert(index < enemies.size());
            return enemies[index];
        }
        
        void createEnemyPool(EnemyMovePool &enemyMovePool) {
            createBlankArray(int(EnemyId::Count));
            
            retrieveForCreation(EnemyId::ShrinkerBeetle, EnemyMoveId::Shrinker, 38, 40, 40, 42);
            retrieveForCreation(EnemyId::FuzzyWormCrawler, EnemyMoveId::AcidGoop, 55, 57, 58, 59);
            retrieveForCreation(EnemyId::Nibbit, EnemyMoveId::Butt, 42, 46, 44, 48);
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
