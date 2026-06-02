#include <iostream>
#include <include.hpp>
#include <test_mcts.cpp>
// #include <test_onnx.cpp>
#include <test_sim.cpp>


int main() {
    test_mcts();

    std::cout << "[Program finished]\n";
    std::string s; std::cin >> s;

    return 0;
}
