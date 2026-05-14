#pragma once

#include <Core/Character/enemy_id.hpp>
#include <Core/Character/character_data.hpp>
#include <Core/Character/Intent/intent_strategy.hpp>


namespace SpireSim {

    struct Enemy {
        EnemyId enemyId = EnemyId::None;
        CharacterData data;
        IntentStrategies intentStrategies;
        int currentIntentStrategy = 0;
        int potentialAdditionalMaxHp = 0;
        bool isMinion = false;

        Enemy() {}
        Enemy(EnemyId enemyId_) : enemyId(enemyId_) {}
        Enemy(EnemyId enemyId_, CharacterData data_) : enemyId(enemyId_), data(data_) {}
        Enemy(EnemyId enemyId_, CharacterData data_, IntentStrategies &intentStrategies_)
            : enemyId(enemyId_), data(data_) {}

        void setMaxHp(int maxHp_, int potentialAdditionalMaxHp_ = 0) {
            data.maxHp = maxHp_; data.hp = maxHp_;
            potentialAdditionalMaxHp = potentialAdditionalMaxHp_;
        }

        std::string toString() {
            std::stringstream ss;
            ss << "EnemyId: " << ToString(enemyId);
            ss << "; Data: [ " << data.toString() << " ]";
            ss << "; Intent Strategies: { ";
            for(auto& e : intentStrategies) ss << "[ " << e.toString() << " ] ";
            ss << "}";
            ss << "; currentIntentStrategy: " << currentIntentStrategy;
            return ss.str();
        }
    };
    using Enemies = std::vector<Enemy>;

}
