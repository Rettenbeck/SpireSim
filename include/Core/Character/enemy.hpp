#pragma once

#include <Core/Character/enemy_id.hpp>
#include <Core/Character/character_data.hpp>


namespace SpireSim {

    struct Enemy {
        EnemyId enemyId = EnemyId::None;
        CharacterData data;
        EnemyMoveIds initialMoves;
        EnemyMoveId currentMove = EnemyMoveId::None;
        int initialMoveCount = 0;
        int potentialAdditionalMaxHp = 0;
        bool isMinion = false;
        EventList eventList;

        Enemy() {}
        Enemy(EnemyId enemyId_) : enemyId(enemyId_) {}
        Enemy(EnemyId enemyId_, CharacterData data_) : enemyId(enemyId_), data(data_) {}
        Enemy(EnemyId enemyId_, CharacterData data_, EnemyMoveId initialMove) : enemyId(enemyId_), data(data_) {
            initialMoves.push_back(initialMove);
        }
        Enemy(EnemyId enemyId_, CharacterData data_, EnemyMoveIds initialMoves_)
            : enemyId(enemyId_), data(data_), initialMoves(initialMoves_) {}

        void setMaxHp(int maxHp_, int potentialAdditionalMaxHp_ = 0) {
            data.maxHp = maxHp_; data.hp = maxHp_;
            potentialAdditionalMaxHp = potentialAdditionalMaxHp_;
        }

        std::string toString() {
            std::stringstream ss;
            ss << "EnemyId: " << ToString(enemyId);
            ss << "; Data: [ " << data.toString() << " ]";
            ss << "; InitialMoves: { ";
            for(auto& e : initialMoves) ss << ToString(e) << " ";
            ss << "}";
            ss << "; InitialMoveCount: " << initialMoveCount;
            return ss.str();
        }
    };
    using Enemies = std::vector<Enemy>;

}
