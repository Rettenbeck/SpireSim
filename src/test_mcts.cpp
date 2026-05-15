#include <iostream>
#include <include.hpp>

void test() {
    int difficultyMoreHp  = 0;  // Ascension 7
    int difficultyMoreDmg = 0;  // Ascension 9
    
    SpireSim::EffectPool        effectPool;
    SpireSim::CardPool          cardPool(effectPool);
    SpireSim::BuffPool          buffPool;
    SpireSim::EnemyMovePool     movePool (buffPool, difficultyMoreDmg);
    SpireSim::EnemyPool         enemyPool(movePool, difficultyMoreHp );
    SpireSim::EncounterFactory  encounterFactory(enemyPool);
    SpireSim::PlayerFactory     playerFactory;

    SpireSim::Cards deck;
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Defend, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Defend, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Defend, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Defend, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Bash  , true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Thunderclap, true));
    
    auto state = std::make_unique<SpireSim::CombatState>(
        effectPool,
        cardPool,
        buffPool,
        movePool,
        enemyPool,
        playerFactory.get(SpireSim::PlayerId::IronClad),
        encounterFactory.get(SpireSim::EncounterId::SF_Test),
        deck);
    
    state->setSeeds(42);
    state->startCombat();

    // SpireSim::MCTS mcts(state.get());
    SpireSim::MCTS_Parallel mcts(state.get());
    mcts.heuristic = std::make_unique<SpireSim::MCTS_Heuristic_Random>(0);
    mcts.optionIterations = 14000;

    std::cout << "Threads: " << std::thread::hardware_concurrency() << "\n";

    for(int i = 0; i < 20; i++) {
        std::cout << state->toString() << "\n\n";
        std::cout << state->actionsToString() << "\n\n";

        std::cout << "Calculating... \n";
        mcts.run();
        int bestActionIndex = mcts.bestActionIndex;
        std::cout << "Best action found: " << bestActionIndex << "\n";
        std::cout << mcts.resultsToString();

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
