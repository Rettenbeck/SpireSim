#pragma once

#include <Core/Character/enemy.hpp>
#include <Core/Character/Intent/enemy_move_pool.hpp>
#include <Core/Character/Intent/intent_strategy.hpp>


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
            
            enemyShrinkerBeetle.intentStrategies.push_back(
                IntentStrategy::createCycle({EnemyMoveId::Shrinker, EnemyMoveId::Chomp, EnemyMoveId::Stomp}, {1, 2}, 0)
            );
            enemyShrinkerBeetle.setMaxHp(38 + 2 * isDifficult, 2);
            
            enemyFuzzyWormCrawler.intentStrategies.push_back(
                IntentStrategy::createCycle({EnemyMoveId::AcidGoop, EnemyMoveId::Inhale}, {0, 1})
            );
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
