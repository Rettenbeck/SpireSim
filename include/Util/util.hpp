#pragma once

#include <Util/basic.hpp>
#include <nlohmann/include.hpp>


namespace SpireSim {

    std::string ToString(int val) { std::stringstream ss; ss << val; return ss.str(); }
    
    template<typename T>
    T FromInt(T type) { return static_cast<T>(type); }

    #define TO_STRING_METHOD \
    std::string toString() { \
        json j; j = *this; return j.dump(); \
    }

    struct RNG {
        unsigned int seed = 0;
        std::mt19937 gen;

        RNG(unsigned int seed_ = 0) {
            setSeed(seed_);
        }

        void setSeed(unsigned int seed_ = 0) {
            seed = seed_;
            gen.seed(seed);
        }

        void increaseSeeds(unsigned int value) {
            setSeed(seed + value);
        }

        int getRandomNumber(int max) { // Including max! e.g. max = 3 produces number ranging from 0 to 3 including
            std::uniform_int_distribution<int> dist(0, max);
            return dist(gen);
        }
    };

}
