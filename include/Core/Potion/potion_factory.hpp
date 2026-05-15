#pragma once

#include <Core/Potion/potion_pool.hpp>


namespace SpireSim {

    struct PotionFactory {
        PotionPool& potionPool;

        PotionFactory(PotionPool &potionPool_) : potionPool(potionPool_) {}

        inline Potion& retrieve(PotionId potionId) {
            return potionPool.retrieve(potionId);
        }

        inline Potions get(const PotionIds &potionIds) {
            Potions potions;
            for(auto& potionId : potionIds) potions.push_back(retrieve(potionId));
            return potions;
        }
    };

}
