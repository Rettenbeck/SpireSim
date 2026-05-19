#pragma once

#include <Core/Combat/include.hpp>


namespace SpireSim {

    struct PoolContainer {
        std::unique_ptr<EffectPool>        effectPool;
        std::unique_ptr<RelicPool>         relicPool;
        std::unique_ptr<PotionPool>        potionPool;
        std::unique_ptr<CardPool>          cardPool;
        std::unique_ptr<BuffPool>          buffPool;
        std::unique_ptr<EnemyMovePool>     movePool;
        std::unique_ptr<EnemyPool>         enemyPool;
        
        std::unique_ptr<EncounterFactory>  encounterFactory;
        std::unique_ptr<PlayerFactory>     playerFactory;
        std::unique_ptr<RelicFactory>      relicFactory;
        std::unique_ptr<PotionFactory>     potionFactory;

        PoolContainer(int difficultyMoreHp = 0, int difficultyMoreDmg = 0) {
            effectPool = std::make_unique<EffectPool>();
            relicPool = std::make_unique<RelicPool>();
            potionPool = std::make_unique<PotionPool>();
            cardPool = std::make_unique<CardPool>(*effectPool);
            buffPool = std::make_unique<BuffPool>();
            movePool = std::make_unique<EnemyMovePool>(*buffPool, difficultyMoreDmg);
            enemyPool = std::make_unique<EnemyPool>(*movePool, difficultyMoreHp);
            encounterFactory = std::make_unique<EncounterFactory>(*enemyPool);
            playerFactory = std::make_unique<PlayerFactory>();
            relicFactory = std::make_unique<RelicFactory>(*relicPool);
            potionFactory = std::make_unique<PotionFactory>(*potionPool);
        }
        
    };
    using UPoolContainer = std::unique_ptr<PoolContainer>;

}
