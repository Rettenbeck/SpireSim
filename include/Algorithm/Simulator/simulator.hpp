#pragma once

#include <Algorithm/Simulator/combat_template.hpp>
#include <Algorithm/implementor.hpp>


namespace SpireSim {

    class Simulator {
    public:
    
        UImplementor implementor = nullptr;
        UCombatTemplates templates;

        std::string optionFilename = "data.txt";
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

            for(int i = 0; i < templates.size(); i++) {
                auto t = templates[i].get();
                t->setSeed(optionInitialSeed + i);
            }

            for(int i = 0; i < optionIterations; i++) {
                int index = getRandomNumber(templates.size() - 1);
                assert(index < templates.size());
                Combat stateCopy = *(templates[index]->get());
                std::cout << "\nFight starts: " << i << "\n";
                std::cout << stateCopy.toString();
                runSingleCombat(&stateCopy);
            }
        }

        void runSingleCombat(Combat *state) {
            assert(state);
            int initialHp = state->getPlayerHealth();
            int hpLoss = 0;
            bool won = false;
            std::vector<json> jvec;

            state->setSeeds(seedBuffer++);
            state->initialize();
            state->startCombat(true);

            for(int i = 0; i < maxActions; i++) {
                if(state->isCombatOver()) {
                    hpLoss = state->getPlayerHealth() - initialHp;
                    won = state->isCombatVictorious();
                    break;
                }

                implementor->initialState = state;
                implementor->run();
                int bestActionIndex = implementor->bestActionIndex;
                assert(bestActionIndex < state->getActions().size());

                std::cout << state->actionsToString() << "bestActionIndex: " << bestActionIndex << "\n";
                std::cout << "Current Hp: " << state->getPlayerHealth() << "\n";

                jvec.push_back(json());
                auto& j = jvec.back();
                j = *state;
                auto& j_scores = j["scores"];
                for(auto [actionIndex, score] : implementor->result.scoreMap) {
                    j_scores.push_back(score);
                }

                state->executeAction(bestActionIndex);
            }

            for(auto& j : jvec) {
                j["hpLoss"] = hpLoss;
                j["won"] = won;
            }

            exportJsonToFile(jvec);
        }

        void exportJsonToFile(std::vector<json> &jvec) {
            std::ofstream file(optionFilename, std::ios::app);
            if (file.is_open()) {
                for(auto& j : jvec) file << j << '\n';;
                file.close();
            } else {
                assert(false);
            }
        }

        void resetFile() {
            std::ofstream file(optionFilename, std::ios::out | std::ios::trunc);
            if (file.is_open()) {
                file << "";
                file.close(); // Datei ist nun leer bis auf diesen einen Eintrag
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
