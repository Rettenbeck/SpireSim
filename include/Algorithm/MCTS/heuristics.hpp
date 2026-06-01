#pragma once

#include <Core/Combat/include.hpp>
#include <Algorithm/algorithm.hpp>


namespace SpireSim {

    class MCTS_Heuristic {
    public:
        virtual int getAction(Combat &state) = 0;
        virtual std::unique_ptr<MCTS_Heuristic> clone() = 0;
        
        RNG rng;

        MCTS_Heuristic(unsigned int seed = 0) {
            rng.setSeed(seed);
        }

    };
    using UMCTS_Heuristic = std::unique_ptr<MCTS_Heuristic>;

    class MCTS_Heuristic_Random : public MCTS_Heuristic {
    public:
        MCTS_Heuristic_Random(unsigned int seed = 0) : MCTS_Heuristic(seed) {}

        int getAction(Combat &state) {
            auto i = rng.getRandomNumber(state.getActions().size() - 1);
            return i;
        }

        std::unique_ptr<MCTS_Heuristic> clone() {
            return std::make_unique<MCTS_Heuristic_Random>(rng.seed);
        }

    };

    class MCTS_Heuristic_Algorithm : public MCTS_Heuristic {
    public:

        UAlgorithm algorithm = nullptr;

        MCTS_Heuristic_Algorithm(UAlgorithm algorithm_, unsigned int seed = 0)
            : algorithm(std::move(algorithm_)), MCTS_Heuristic(seed) {}

        int getAction(Combat &state) {
            assert(algorithm);
            algorithm->initialState = &state;
            algorithm->run();
            return algorithm->bestActionIndex;
        }
        
        std::unique_ptr<MCTS_Heuristic> clone() {
            return std::make_unique<MCTS_Heuristic_Random>(rng.seed);
        }

    };

}
