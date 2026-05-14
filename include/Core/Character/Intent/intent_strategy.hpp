#pragma once

#include <Core/Character/Intent/intent_strategy_type.hpp>
#include <Core/Character/Intent/enemy_move_id.hpp>


namespace SpireSim {

    const int INTENT_NONE = -1;

    struct IntentStrategy {
        IntentStrategyType strategyType;
        EnemyMoveIds enemyMoveIds;
        int firstIntent = INTENT_NONE;
        int lastIntent = INTENT_NONE;
        
        std::vector<int> cycle;
        int cycleIndex = -1;

        inline int generateNextIntentIdCycle() {
            cycleIndex++;
            if(cycleIndex >= cycle.size()) cycleIndex = 0;
            return cycleIndex;
        }

        inline int generateNextIntentId() {
            switch(strategyType) {
                case IntentStrategyType::Cycle: return generateNextIntentIdCycle();
                default: assert(false);
            }
            return -1;
        }

        EnemyMoveId generateNextIntent() {
            int index;
            if(firstIntent != INTENT_NONE && lastIntent == INTENT_NONE) {
                index = firstIntent;
            } else {
                index = generateNextIntentId();
            }
            assert(index < enemyMoveIds.size());
            lastIntent = index;
            return enemyMoveIds[index];
        }

        IntentStrategy(IntentStrategyType strategyType_ = IntentStrategyType::None)
            : strategyType(strategyType_) {}

        IntentStrategy(const EnemyMoveIds enemyMoveIds_, IntentStrategyType strategyType_ = IntentStrategyType::None)
            : strategyType(strategyType_), enemyMoveIds(enemyMoveIds_) {}

        static IntentStrategy createCycle(const EnemyMoveIds enemyMoveIds_, const std::vector<int> cycle_, int firstIntent_ = INTENT_NONE) {
            IntentStrategy intentStrategy(enemyMoveIds_, IntentStrategyType::Cycle);
            intentStrategy.cycle = cycle_;
            intentStrategy.firstIntent = firstIntent_;
            return intentStrategy;
        }

        std::string toStringCycle() {
            std::stringstream ss;
            ss << "Cycle index: " << cycleIndex << "; Cycle: { ";
            for(auto e : cycle) ss << e << " ";
            ss << "} ";
            return ss.str();
        }

        std::string toString() {
            std::stringstream ss;
            ss << "strategyType: " << ToString(strategyType) << "; firstIntent: " << firstIntent
                << "; lastIntent: " << lastIntent << "; EnemyMoveIds: { ";
            for(auto& enemyMoveId : enemyMoveIds) ss << ToString(enemyMoveId) << " ";
            ss << "}";
            
            switch(strategyType) {
                case IntentStrategyType::Cycle:
                    ss << "; " << toStringCycle();
                    break;
            }

            return ss.str();
        }
    };
    using IntentStrategies = std::vector<IntentStrategy>;

}
