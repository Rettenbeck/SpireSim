#pragma once

#include <Core/Character/encounter_id.hpp>
#include <Core/Character/enemy_pool.hpp>


namespace SpireSim {

    struct EncounterFactory {
        EnemyPool& enemyPool;

        EncounterFactory(EnemyPool &enemyPool_) : enemyPool(enemyPool_) {}

        inline Enemy& retrieve(EnemyId enemyId) {
            return enemyPool.retrieve(enemyId);
        }

        inline Enemies get(const EnemyIds &enemyIds) {
            Enemies enemies;
            for(auto& enemyId : enemyIds) enemies.push_back(retrieve(enemyId));
            return enemies;
        }

        Enemies get(EncounterId encounterId) {
            switch(encounterId) {
                case EncounterId::ShrinkerBeetleWeak  : return get({EnemyId::ShrinkerBeetle  });
                case EncounterId::FuzzyWormCrawlerWeak: return get({EnemyId::FuzzyWormCrawler});
                case EncounterId::SF_Test: return get({EnemyId::ShrinkerBeetle, EnemyId::FuzzyWormCrawler});
                default: assert(false);
            }
        }
    };

}
