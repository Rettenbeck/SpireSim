#include <iostream>
#include <include.hpp>

void test() {
    //auto ent = new SpireSim::Entity(32);

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
    
    SpireSim::CombatState state(
        effectPool,
        cardPool,
        buffPool,
        movePool,
        enemyPool,
        playerFactory.get(SpireSim::PlayerId::IronClad),
        encounterFactory.get(SpireSim::EncounterId::SF_Test),
        deck);
    
    state.setSeeds(42);

    // SpireSim::Ids lst = {6, 4, 0, 0, 0, 1};
    
    // for(auto id : lst) {
    //     std::cout << "-----------------------------------------------------------\n";
    //     std::cout << "  PLAYING CARD: " << id << "\n";
        
    //     std::cout << state.stackToString() << "\n";
    //     state.playCard(id, 8);
    //     std::cout << state.stackToString() << "\n";
    //     state.resolveStackFully();
        
    //     std::cout << state.toString() << "\n";

        
    //     std::cout << "    -------Enemy move----------------\n";
    //     state.executeIntents();
    //     std::cout << state.toString() << "\n";
    // }

    SpireSim::CombatState state2 = state;

    for(int i = 0; i < 20; i++) {
        state.proceedPhases();
        std::cout << state.toString() << "\n\n";
        std::cout << state.actionsToString() << "\n\n";

        std::string s;
        std::cin >> s;
        if(s == "x") return;

        int val = -1;

        try {
            val = stoi(s);
        } catch(const std::exception& e) {
            val = -1;
        }
        
        if(val < 0) continue;
        std::cout << "Chosen: " << val << "\n";

        state.executeAction(val);
        
        if(state.isCombatOver()) {
            std::cout << "FIGHT OVER\n";
            return;
        }
    }
    
    
    // SpireSim::Enchantment e(SpireSim::EnchantmentType::Sharp, 3);
    // SpireSim::Modification m1(SpireSim::ModificationType::Exhaust, 1);
    // SpireSim::Modification m2(SpireSim::ModificationType::Ethereal, 5);
    
    // auto card = cardPool.createCardFromTemplate(SpireSim::CardId::Defend, true);
    // card.addModification(m1);
    // card.addModification(m2);
    
    // std::cout << card.toString() << "\n";

}
