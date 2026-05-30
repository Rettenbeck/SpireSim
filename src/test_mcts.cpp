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

    auto mcts = std::make_unique<SpireSim::MCTS>(state, std::make_unique<SpireSim::MCTS_Heuristic_Random>(0));
    mcts->optionIterations = 2600;
    mcts->optionCombats = 1;
    mcts->optionNumberThreads = 1;

    for(int i = 0; i < 80; i++) {
        std::cout << state->toString() << "\n\n";
        std::cout << state->actionsToString() << "\n\n";

        std::cout << "Calculating... \n";
        assert(mcts); mcts->run();
        std::cout << mcts->toString();

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
