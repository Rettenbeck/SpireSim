#include <iostream>
#include <include.hpp>
#include <test_data.cpp>

void test_sim() {

    SpireSim::PoolContainer container;
    auto templates = createTestData(container);
    assert(templates.size() > 0);
    
    auto mcts = std::make_unique<SpireSim::MCTS>(std::make_unique<SpireSim::MCTS_Heuristic_Random>(0));
    mcts->optionIterations = 1600;
    mcts->optionCombats = 1;
    mcts->optionNumberThreads = 1;

    auto implementor = std::make_unique<SpireSim::Implementor>(std::move(mcts));
    implementor->optionIterations = 1;
    implementor->optionNumberThreads = 1;

    SpireSim::Simulator simulator(std::move(implementor));
    simulator.optionIterations = 90;
    simulator.setTemplates(templates);
    simulator.run();

}
