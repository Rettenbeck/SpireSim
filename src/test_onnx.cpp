#include <iostream>
#include <include.hpp>
#include <test_data.cpp>

void test_onnx() {

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

    auto onnx = std::make_unique<SpireSim::ONNX>(state);

    for(int i = 0; i < 20; i++) {
        std::cout << state->toString() << "\n\n";
        std::cout << state->actionsToString() << "\n\n";

        std::cout << "Calculating... \n";

        assert(onnx); onnx->run();
        std::cout << onnx->resultToString();
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
