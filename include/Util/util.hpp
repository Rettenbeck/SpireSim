#pragma once

#include <Util/basic.hpp>
#include <nlohmann/include.hpp>
// #include <windows.h>


namespace SpireSim {

    std::string ToString(int val) { std::stringstream ss; ss << val; return ss.str(); }
    
    template<typename T>
    T FromInt(T type) { return static_cast<T>(type); }

    #define TO_STRING_METHOD \
    std::string toString() { \
        json j; j = *this; return j.dump(); \
    }

    // void print_stacktrace() {
    //     void* stack[100];
    //     unsigned short frames = CaptureStackBackTrace(0, 100, stack, NULL);
    //     std::cerr << "Triggered assert. Frames captured: " << frames << "\n";
    // }

    // #define custom_assert(condition)    \
    //     if(!condition) {                \
    //         print_stacktrace();         \
    //     }

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
            assert(max >= 0);
            switch (max) {
                case 0: return 0;
                case 1: return gen() & 1;
                case 2: return gen() % 3;
                default:
                    std::uniform_int_distribution<int> dist(0, max);
                    return dist(gen);
            }
        }
    };

}
