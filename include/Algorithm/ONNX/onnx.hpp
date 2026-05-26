#pragma once

#include <Algorithm/algorithm.hpp>
#include <onnxruntime_cxx_api.h>


namespace SpireSim {

    using Floats = std::vector<float>;
    using Int64s = std::vector<int64_t>;
    using Pair = std::pair<Floats, Int64s>;

    class ONNX : public Algorithm {
    public:
    
        std::unique_ptr<Ort::Session> session = nullptr;
        Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
        Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "SpireSimInference"};
        Ort::SessionOptions session_options;

        ONNX() {
            initialize();
        }

        ONNX(Combat *initialState_) : Algorithm(initialState_) {
            initialize();
        }

        void run() {
            assert(session);
            assert(initialState);
            result.clear();
            probabilities.clear();
            bestActionIndex = -1;

            auto global_data = getGlobalData();
            auto enemy_data  = getEnemyData();
            auto player_data = getPlayerData();
            auto card_data   = getCardData();

            checkInput(global_data);
            checkInput(enemy_data );
            checkInput(player_data);
            checkInput(card_data  );
            
            auto& actions = initialState->getActions();
            double bestActionScore = -99999;
            for(int actionIndex = 0; actionIndex < actions.size(); actionIndex++) {
                auto& action = actions[actionIndex];
                auto estimate = estimateAction(action, global_data, enemy_data, player_data, card_data);

                result.scoreMap[actionIndex] = estimate;
                if(estimate > bestActionScore) {
                    bestActionScore = estimate;
                    bestActionIndex = actionIndex;
                }
            }
        }

        float estimateAction(const Action &action, Pair &global_data, Pair &enemy_data, Pair &player_data, Pair &card_data) {
            auto action_data = getActionData(action);
            checkInput(action_data);

            std::vector<Ort::Value> input_tensors;
            input_tensors.push_back(Ort::Value::CreateTensor<float>(
                memory_info,
                global_data.first.data(), global_data.first.size(), global_data.second.data(), global_data.second.size()
            ));

            input_tensors.push_back(Ort::Value::CreateTensor<float>(
                memory_info,
                enemy_data.first.data(), enemy_data.first.size(), enemy_data.second.data(), enemy_data.second.size()
            ));

            input_tensors.push_back(Ort::Value::CreateTensor<float>(
                memory_info,
                player_data.first.data(), player_data.first.size(), player_data.second.data(), player_data.second.size()
            ));
            
            input_tensors.push_back(Ort::Value::CreateTensor<float>(
                memory_info,
                card_data.first.data(), card_data.first.size(), card_data.second.data(), card_data.second.size()
            ));
            
            input_tensors.push_back(Ort::Value::CreateTensor<float>(
                memory_info,
                action_data.first.data(), action_data.first.size(), action_data.second.data(), action_data.second.size()
            ));

            // 4. Namen der Inputs und Outputs festlegen (Exakt wie im Python-Export definiert!)
            const char* input_names[] = { "global_vars", "enemy_feats", "player_feats", "hand_cards", "action_feats" };
            const char* output_names[] = { "score" };

            // std::cout << "Führe Inferenz aus..." << std::endl;
            auto output_tensors = session->Run(
                Ort::RunOptions{ nullptr },
                input_names,
                input_tensors.data(),
                input_tensors.size(),
                output_names,
                1
            );

            // 6. Output auslesen
            float* float_array = output_tensors[0].GetTensorMutableData<float>();
            float final_score = float_array[0];

            // std::cout << "MCTS-Score Vorhersage der KI: " << final_score << std::endl;
            return final_score;
        }

        Pair getGlobalData() {
            Pair data;
            data.first = {
                normalize(initialState->variables.turn, 10.0),
                normalize(initialState->variables.enemies, 4.0)
            };
            data.second = { 1, 2 };
            return data;
        }

        Pair getEnemyData() {
            Pair data;
            auto& ecs = initialState->ecs;
            auto& enemies = ecs.cEnemies;

            std::vector<Id> enemyIds;
            for(auto id = 0; id < enemies.size(); id++) {
                if(ecs.isEnemy(id)) enemyIds.push_back(id);
            }

            for(int i = 0; i < 4; i++) {
                if(i < enemyIds.size()) {
                    auto enemyId = enemyIds[i];
                    assert(enemyId < enemies.size());
                    auto& enemy = enemies[enemyId];

                    data.first.push_back(enemyId);
                    data.first.push_back(int(enemy.enemyId));
                    data.first.push_back(int(enemy.currentMove));
                    data.first.push_back(normalize(enemy.data.hp, 100.0));
                    data.first.push_back(normalize(enemy.data.block, 100.0));
                    data.first.push_back(normalize(enemy.data.strength, 10.0));
                } else {
                    data.first.push_back(-1);
                    data.first.push_back(-1);
                    data.first.push_back(-1);
                    data.first.push_back(0.0);
                    data.first.push_back(0.0);
                    data.first.push_back(0.0);
                }
            }
            data.second = { 1, 4, 6 };
            return data;
        }

        Pair getPlayerData() {
            Pair data;
            auto& playerData = initialState->ecs.getPlayer().data;
            data.first = {
                normalize(playerData.hp, 100.0),
                normalize(playerData.block, 100.0),
                normalize(initialState->variables.energy, 3.0),
                normalize(initialState->variables.stars, 5.0)
            };
            data.second = { 1, 4 };
            return data;
        }

        Pair getCardData() {
            Pair data;
            auto& ecs = initialState->ecs;
            auto& piles = initialState->pileHandler;
            auto& cards = ecs.cCards;

            for(int i = 0; i < 10; i++) {
                if(i < piles.hand.size()) {
                    auto cardEntityId = piles.hand[i];
                    auto& card = cards[cardEntityId];

                    data.first.push_back(cardEntityId);
                    data.first.push_back(int(card.cardId));
                    data.first.push_back(normalize(card.data.damage, 20.0));
                    data.first.push_back(normalize(card.data.block, 20.0));
                    data.first.push_back(card.data.energyCost);
                    data.first.push_back(card.data.starCost);
                    data.first.push_back(card.data.vulnerable);
                    data.first.push_back(card.data.weak);
                    data.first.push_back((card.isUpgraded ? 1.0 : 0.0));
                } else {
                    data.first.push_back(-1);
                    data.first.push_back(-1);
                    data.first.push_back(0.0);
                    data.first.push_back(0.0);
                    data.first.push_back(0.0);
                    data.first.push_back(0.0);
                    data.first.push_back(0.0);
                    data.first.push_back(0.0);
                    data.first.push_back(0.0);
                }
            }
            data.second = { 1, 10, 9 };
            return data;
        }

        Pair getActionData(const Action &action) {
            Pair data;
            data.first = {
                (float) action.actionType,
                (float) action.entityToPlay,
                (float) action.targetEntityId
            };
            data.second = { 1, 3 };
            return data;
        }

        float normalize(float value, float normal) {
            return value / normal;
        }

        void checkInput(Pair &data) {
            int input_size = 1;
            for(auto value : data.second) input_size *= value;
            if(data.first.size() != input_size) {
                std::cout << data.first.size() << "; " << input_size << "\n";
                assert(false);
            }
        }

        void initialize() {
            session_options.SetIntraOpNumThreads(1);
    
            #ifdef _WIN32
                const wchar_t* model_path = L"ecs_ai_model.onnx";
            #else
                const char* model_path = "ecs_ai_model.onnx";
            #endif

            // std::cout << "Lade Modell..." << std::endl;
            session = std::make_unique<Ort::Session>(env, model_path, session_options);
        }

        UAlgorithm clone() {
            return std::make_unique<ONNX>();
        }

    };
    using U_ONNX = std::unique_ptr<ONNX>;

}
