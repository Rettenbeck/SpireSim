#pragma once

#include <Core/Combat/entity.hpp>


namespace SpireSim {

    struct Stats {
        // Id id = ENTITY_NONE;
        long countPlayable = 0;
        long countPlayed = 0;
        float countPlayedWeighted = 0.f;
    };

}
