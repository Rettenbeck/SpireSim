#pragma once

#include <Algorithm/Simulator/combat_template.hpp>
#include <Algorithm/implementor.hpp>


namespace SpireSim {

    class Simulator {
    public:
    
        UImplementor implementor = nullptr;
        UCombatTemplates templates;

        CardStatsMap cardStatsMap;

        std::string optionFilename = "data.txt";
        std::string optionFilenameCardStats = "stats.txt";
        unsigned int optionInitialSeed = 1;
        int optionIterations = 1;
        unsigned int seedBuffer = 0;
        unsigned int maxActions = 999;
        std::mt19937 rng;

        Simulator() {}
        Simulator(UImplementor implementor_) : implementor(std::move(implementor_)) {}
        
        void run() {
            assert(templates.size() > 0);
            assert(implementor);
            resetFile();

            rng.seed(optionInitialSeed);
            seedBuffer = optionInitialSeed;
            cardStatsMap.clear();

            for(int i = 0; i < templates.size(); i++) {
                auto t = templates[i].get();
                t->setSeed(optionInitialSeed + i);
            }

            for(int i = 0; i < optionIterations; i++) {
                int index = getRandomNumber(templates.size() - 1);
                assert(index < templates.size());
                Combat stateCopy = *(templates[index]->get());
                std::cout << "\nFight starts: " << i << "\n";

                CardStatsMap cardStatsMapSingle;
                runSingleCombat(&stateCopy, cardStatsMapSingle);
                Merge(cardStatsMap, cardStatsMapSingle);
                std::cout << "Card stats total: " << ToStringCardId(cardStatsMap, &stateCopy);
                exportCardStatsToFile();
            }
        }

        void runSingleCombat(Combat *state, CardStatsMap &cardStatsMapSingle) {
            assert(state);
            int initialHp = state->getPlayerHealth();
            int hpLoss = 0;
            bool won = false;
            float score = 0.f;
            std::vector<json> jvec;

            state->setSeeds(seedBuffer++);
            state->initialize();
            std::cout << state->toString();
            state->startCombat();

            for(int i = 0; i < maxActions; i++) {
                if(state->isCombatOver()) {
                    hpLoss = initialHp - state->getPlayerHealth();
                    won = state->isCombatVictorious();
                    score = ((float) state->getPlayerHealth()) / ((float) state->getPlayerMaxHealth());
                    if(score < 0.f) score = 0.f;
                    break;
                }

                implementor->initialState = state;
                implementor->run();
                int bestActionIndex = implementor->bestActionIndex;
                assert(bestActionIndex < state->getActions().size());

                std::cout << "\n";
                std::cout << state->actionsToString();
                std::cout << implementor->result.toString();
                std::cout << "bestActionIndex: " << bestActionIndex << "\n";
                std::cout << "Current Hp: " << state->getPlayerHealth() << "\n";

                jvec.push_back(json());
                auto& j = jvec.back();
                j["state"] = *state;
                auto& j_actions = j["actions"];
                auto& actions = state->getActions();
                for(auto [actionIndex, score] : implementor->result.scoreMap) {
                    assert(actionIndex < actions.size());
                    auto& action = actions[actionIndex];
                    json j_action;
                    j_action["score"] = score;
                    j_action["action"] = actions[actionIndex];
                    j_actions.push_back(j_action);
                    
                    if(action.actionType == ActionType::PlayCard) {
                        auto cardId = state->getCardIdFromEntityId(action.entityToPlay);
                        auto& stats = cardStatsMapSingle[int(cardId)];
                        stats.countPlayable++;
                        if(actionIndex == bestActionIndex) stats.countPlayedIfAble++;
                    }
                }

                state->executeAction(bestActionIndex);
            }

            for(auto& [cardEntityId, stats] : cardStatsMapSingle) {
                stats.countPlayedIfAbleWeighted = stats.countPlayedIfAble * score;
            }

            const auto& drawnMap = state->getDrawnMap();
            const auto& discardedMap = state->getDiscardedMap();
            
            for(const auto& [cardEntityId, count] : drawnMap) {
                auto cardId = state->getCardIdFromEntityId(cardEntityId);
                auto& stats = cardStatsMapSingle[int(cardId)];
                stats.countDrawn = count;
            }

            for(const auto& [cardEntityId, count] : discardedMap) {
                auto cardId = state->getCardIdFromEntityId(cardEntityId);
                auto& stats = cardStatsMapSingle[int(cardId)];
                stats.countDiscarded = count;
            }

            std::cout << "Fight finished! Won? " << won << "; Score: " << score << "\n";
            std::cout << "Card stats single: " << ToStringCardId(cardStatsMapSingle, state);

            for(auto& j : jvec) {
                j["hpLoss"] = hpLoss;
                j["won"] = won;
            }

            exportJsonToFile(jvec);
        }

        void exportJsonToFile(std::vector<json> &jvec) {
            std::ofstream file(optionFilename, std::ios::app);
            if (file.is_open()) {
                for(auto& j : jvec) file << j.dump() << '\n';
                file.close();
            } else {
                assert(false);
            }
        }

        void resetFile() {
            std::ofstream file(optionFilename, std::ios::out | std::ios::trunc);
            if (file.is_open()) {
                file << "";
                file.close();
            }
        }

        void exportCardStatsToFile() {
            std::ofstream file(optionFilenameCardStats);
            if (file.is_open()) {
                json j;
                j = cardStatsMap;
                file << j.dump(2) << '\n';
                file.close();
            } else {
                assert(false);
            }
        }

        void setTemplates(UCombatTemplates &vec) {
            templates.clear();
            templates.insert(
                templates.end(),
                std::make_move_iterator(vec.begin()),
                std::make_move_iterator(vec.end())
            );
        }

        int getRandomNumber(int max) { // Including max! E.g. max = 3 produces numbers ranging from 0 to 3 including
            std::uniform_int_distribution<int> dist(0, max);
            return dist(rng);
        }

    };

}
