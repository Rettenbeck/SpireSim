#pragma once

#include <Core/Relic/relic_pool.hpp>


namespace SpireSim {

    struct RelicFactory {
        RelicPool& relicPool;

        RelicFactory(RelicPool &relicPool_) : relicPool(relicPool_) {}

        inline Relic& retrieve(RelicId relicId) {
            return relicPool.retrieve(relicId);
        }

        inline Relics get(const RelicIds &relicIds) {
            Relics relics;
            for(auto& relicId : relicIds) relics.push_back(retrieve(relicId));
            return relics;
        }
    };

}
