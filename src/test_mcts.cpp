#include <iostream>
#include <include.hpp>
#include <test_data.cpp>

void test_mcts() {

    int difficultyMoreHp  = 0;  // Ascension 7
    int difficultyMoreDmg = 0;  // Ascension 9
    
    SpireSim::PoolContainer container;
    auto templates = createTestData(container);
    assert(templates.size() > 0);
    auto state = templates[0]->get();
    assert(state);

    state->setSeeds(42);
    state->initialize();
    state->startCombat();

    // auto mcts_inner = std::make_unique<SpireSim::MCTS_Pass>(std::make_unique<SpireSim::MCTS_Heuristic_Random>(0));
    // mcts_inner->optionIterations = 520;
    // auto heuristic = std::make_unique<SpireSim::MCTS_Heuristic_Algorithm>(std::move(mcts_inner), 0);

    // auto mcts = std::make_unique<SpireSim::MCTS_Pass>(state, std::move(heuristic));
    // mcts->optionIterations = 110;


    auto mcts = std::make_unique<SpireSim::MCTS>(state, std::make_unique<SpireSim::MCTS_Heuristic_Random>(0));
    mcts->optionIterations = 2600;
    mcts->optionCombats = 1;
    mcts->optionNumberThreads = 1;

    // SpireSim::Implementor implementor(state, std::move(mcts));
    // implementor.optionIterations = 12;
    // implementor.optionNumberThreads = 12;

    for(int i = 0; i < 20; i++) {
        std::cout << state->toString() << "\n\n";
        std::cout << state->actionsToString() << "\n\n";
        // state->exportJsonToFile();

        std::cout << "Calculating... \n";

        assert(mcts); mcts->run();
        std::cout << mcts->toString();
        // implementor.run();
        // std::cout << implementor.toString();

        std::string s;
        std::cin >> s;
        if(s == "x") return;

        int val = -1;

        try {
            val = stoi(s);
        } catch(const std::exception& e) {
            val = -1;
        }
        
        if(val < 0) {
            state->reshuffleDeck();
            continue;
        }
        std::cout << "Chosen: " << val << "\n";

        state->executeAction(val);
        
        if(state->isCombatOver()) {
            std::cout << "FIGHT OVER\n";
            return;
        }
    }
    
}
