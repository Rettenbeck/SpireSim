#pragma once

#include <Core/Combat/include.hpp>
#include <Algorithm/algorithm.hpp>


namespace SpireSim {

    class MCTS_Heuristic {
    public:
        virtual int getAction(Combat &state) = 0;
        virtual std::unique_ptr<MCTS_Heuristic> clone() = 0;
        
        void setSeed(unsigned int seed) {
            optionSeed = seed;
            rng.seed(optionSeed);
        }

        MCTS_Heuristic(unsigned int seed = 0) {
            setSeed(seed);
        }

        int getRandomNumber(int max) { // Including max! E.g. max = 3 produces numbers ranging from 0 to 3 including
            std::uniform_int_distribution<int> dist(0, max);
            return dist(rng);
        }

        unsigned int optionSeed = 0;
        std::mt19937 rng;

    };
    using UMCTS_Heuristic = std::unique_ptr<MCTS_Heuristic>;

    class MCTS_Heuristic_Random : public MCTS_Heuristic {
    public:
        MCTS_Heuristic_Random(unsigned int seed = 0) : MCTS_Heuristic(seed) {}

        int getAction(Combat &state) {
            auto i = getRandomNumber(state.getActions().size() - 1);
            // std::cout << i << "; size: " << state.getActions().size() << " ; ";
            return i;
        }

        std::unique_ptr<MCTS_Heuristic> clone() {
            return std::make_unique<MCTS_Heuristic_Random>(optionSeed);
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
            return std::make_unique<MCTS_Heuristic_Random>(optionSeed);
        }

    };

}
