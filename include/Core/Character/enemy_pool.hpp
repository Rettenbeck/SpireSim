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
        
        Enemy& retrieveForCreation(EnemyId enemyId) {
            int index = int(enemyId);
            createBlankArray(index);
            enemies[index].enemyId = enemyId;
            return enemies[index];
        }
        
        inline Enemy& retrieve(EnemyId enemyId) {
            int index = int(enemyId);
            assert(index < enemies.size());
            return enemies[index];
        }
        
        void createEnemyPool(EnemyMovePool &enemyMovePool) {
            createBlankArray(int(EnemyId::Count));
            auto& enemyShrinkerBeetle = retrieveForCreation(EnemyId::ShrinkerBeetle);
            auto& enemyFuzzyWormCrawler = retrieveForCreation(EnemyId::FuzzyWormCrawler);
            
            enemyShrinkerBeetle.initialMoves.push_back(EnemyMoveId::Shrinker);
            enemyShrinkerBeetle.setMaxHp(38 + 2 * isDifficult, 2);
            
            enemyFuzzyWormCrawler.initialMoves.push_back(EnemyMoveId::AcidGoop);
            enemyFuzzyWormCrawler.setMaxHp(55 + 2 * isDifficult, 2);
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
