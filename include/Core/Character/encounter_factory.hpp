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

        Enemies get(const EnemyIds &enemyIds, std::vector<int> advanceMoves) {
            auto enemies = get(enemyIds);
            assert(advanceMoves.size() <= enemies.size());
            for(int i = 0; i < advanceMoves.size(); i++) {
                int& advanceBy = advanceMoves[i];
                if(advanceBy == 0) continue;

                auto& initialMoves = enemies[i].initialMoves;
                assert(initialMoves.size() > 0);
                auto& initialMove = initialMoves[0];

                for(int j = 0; j < advanceBy; j++) {
                    auto it = NextEnemyMoveList.find(initialMove);
                    assert(it != NextEnemyMoveList.end());
                    auto& nextMoves = it->second;
                    assert(nextMoves.size() > 0);
                    initialMove = nextMoves[0];
                }
            }
            return enemies;
        }

        Enemies get(EncounterId encounterId) {
            switch(encounterId) {
                case EncounterId::ShrinkerBeetleWeak  : return get({EnemyId::ShrinkerBeetle  });
                case EncounterId::FuzzyWormCrawlerWeak: return get({EnemyId::FuzzyWormCrawler});
                case EncounterId::SF_Test: return get({EnemyId::ShrinkerBeetle, EnemyId::FuzzyWormCrawler});
                case EncounterId::Nibbits: return get({EnemyId::Nibbit, EnemyId::Nibbit}, {1, 2});
                default: assert(false);
            }
        }
    };

}
