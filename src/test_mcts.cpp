#include <iostream>
#include <include.hpp>

void test() {

    int difficultyMoreHp  = 0;  // Ascension 7
    int difficultyMoreDmg = 0;  // Ascension 9
    
    SpireSim::EffectPool        effectPool;
    SpireSim::RelicPool         relicPool;
    SpireSim::PotionPool        potionPool;
    SpireSim::CardPool          cardPool(effectPool);
    SpireSim::BuffPool          buffPool;
    SpireSim::EnemyMovePool     movePool (buffPool, difficultyMoreDmg);
    SpireSim::EnemyPool         enemyPool(movePool, difficultyMoreHp );
    
    SpireSim::EncounterFactory  encounterFactory(enemyPool);
    SpireSim::PlayerFactory     playerFactory;
    SpireSim::RelicFactory      relicFactory(relicPool);
    SpireSim::PotionFactory     potionFactory(potionPool);

    SpireSim::Cards deck;
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::BloodLetting, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Strike, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Defend, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Defend, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Defend, true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::FallingStar  , true));
    deck.push_back(cardPool.createCardFromTemplate(SpireSim::CardId::Venerate, true));
    
    auto state = std::make_unique<SpireSim::Combat>(
        effectPool,
        relicPool,
        potionPool,
        cardPool,
        buffPool,
        movePool,
        enemyPool,
        playerFactory.get(SpireSim::PlayerId::IronClad),
        encounterFactory.get(SpireSim::EncounterId::Nibbits),
        relicFactory.get({SpireSim::RelicId::BagOfPreparation, SpireSim::RelicId::CentennialPuzzle}),
        potionFactory.get({SpireSim::PotionId::FirePotion, SpireSim::PotionId::FlexPotion}),
        deck);
    
    state->setSeeds(42);
    state->initialize();
    state->startCombat();

    auto mcts = std::make_unique<SpireSim::MCTS>(state.get(), std::make_unique<SpireSim::MCTS_Heuristic_Random>(0));
    mcts->optionIterations = 12000;
    mcts->optionCombats = 1;
    mcts->optionNumberThreads = 1;

    SpireSim::Implementor implementor(state.get(), std::move(mcts));
    implementor.optionIterations = 12;
    implementor.optionNumberThreads = 12;

    for(int i = 0; i < 20; i++) {
        std::cout << state->toString() << "\n\n";
        std::cout << state->actionsToString() << "\n\n";
        state->exportJsonToFile();

        std::cout << "Calculating... \n";

        // assert(mcts); mcts->run();
        // std::cout << mcts->toString();
        implementor.run();
        std::cout << implementor.toString();

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
