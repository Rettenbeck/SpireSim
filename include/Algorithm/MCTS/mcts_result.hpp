#pragma once

#include <Core/Combat/entity.hpp>


namespace SpireSim {

    struct MCTS_Result {
        double visits = 0;
        double totalScore = 0;
    };

}
