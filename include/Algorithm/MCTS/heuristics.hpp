#pragma once

#include <Core/Combat/include.hpp>


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

    class MCTS_Heuristic_Random : public MCTS_Heuristic {
    public:
        MCTS_Heuristic_Random(unsigned int seed) : MCTS_Heuristic(seed) {}

        int getAction(Combat &state) {
            return getRandomNumber(state.getActions().size() - 1);
        }

        std::unique_ptr<MCTS_Heuristic> clone() {
            return std::make_unique<MCTS_Heuristic_Random>(optionSeed);
        }

    };

}
