#pragma once

#include <Core/Character/Move/enemy_move_id.hpp>


namespace SpireSim {

    std::map<EnemyMoveId, EnemyMoveIds> NextEnemyMoveList;

    void fillNextEnemyMoveList() {
        /* Shrinker Beetle */
        NextEnemyMoveList[EnemyMoveId::Shrinker].push_back(EnemyMoveId::Chomp);
        NextEnemyMoveList[EnemyMoveId::Chomp   ].push_back(EnemyMoveId::Stomp);
        NextEnemyMoveList[EnemyMoveId::Stomp   ].push_back(EnemyMoveId::Chomp);

        /* Fuzzy Worm Crawler */
        NextEnemyMoveList[EnemyMoveId::AcidGoop].push_back(EnemyMoveId::Inhale  );
        NextEnemyMoveList[EnemyMoveId::Inhale  ].push_back(EnemyMoveId::AcidGoop);
    }

}
