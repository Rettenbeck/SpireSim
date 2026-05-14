#pragma once

#include <Core/Card/card_pool.hpp>
#include <Core/Buff/buff_pool.hpp>
#include <Core/Character/Move/enemy_move_pool.hpp>
#include <Core/Character/enemy_pool.hpp>
#include <Core/Combat/combat_variables.hpp>
#include <Core/Combat/pile_handler.hpp>
#include <Core/Combat/turn_id.hpp>
#include <Core/Combat/Action/action.hpp>


namespace SpireSim {

    class CombatState {
    private:

        ECS ecs;
        CombatVariables variables;
        PileHandler pileHandler;
        
        EffectPool& effectPool;
        CardPool& cardPool;
        BuffPool& buffPool;
        EnemyMovePool& movePool;
        EnemyPool& enemyPool;
        
        EventRegistry eventRegistry;
        
        Effects stack;

        TurnId turnId = TurnId::None;

        bool waitingForActionOnStack = false;
        bool waitingForAction = false;
        bool stopStack = false;
        bool combatOver = false;
        Actions actions;

        unsigned int seedDeck = 0;
        std::mt19937 genDeck;
        


        // Initializing
        void registerCardEvents(Id id, CardId cardId);
        void initializeCards(Cards &cards);
        void registerEnemies(const Enemies &enemies);
        


        // Stack handling
        void putEffectOntoStack(const Effect &effect, int position = 0);
        void putCardOntoStack(Id cardEntityId, Id targetEntityId = TARGET_RANDOM);
        void triggerEvent(EventType eventType);
        void resolve(Effect &effect);
        


        // Params & Conditions
        inline int resolveParam(Param &param, Id sourceEntityId);
        inline void resolveParams(Effect &effect);
        inline bool evaluateCondition(Condition &condition, Id sourceEntityId);
        inline bool evaluateConditions(Effect &effect);
        


        // Fight effects
        void endFight(bool won);
        void checkForFightEnd();
        void entityDies(Id entityId);

        inline int calculateDamage( Id sourceEntityId, CharacterData &sourceData,
                                    Id targetEntityId, CharacterData &targetData,
                                    int damage);

        void dealDamage(Id sourceEntityId, CharacterData &sourceData,
                        Id targetEntityId, CharacterData &targetData,
                        int damage);

        inline void gainBlock(CharacterData &data, int block);
        inline void gainBlockPlayer(int block);
        inline void gainBlockEnemy(Enemy &enemy, int block);
        inline void gainBlockEnemy(Id &enemyEntityId, int block);
        void gainBlock(Id entityId, int block);

        inline void dealDamageToEnemy(Id sourceEntityId, CharacterData &sourceData, Id targetEntityId, int damage);
        inline void gainStrength(CharacterData &targetData, int value);
        inline void gainStrengthEnemy(Id targetEntityId, int value);
        inline void gainStrengthPlayer(int value);
        inline void gainDexterity(CharacterData &targetData, int value);
        inline void gainDexterityPlayer(int value);
        inline void applyVulnerable(CharacterData &targetData, int value);
        inline void applyVulnerableToEnemy(Id targetEntityId, int value);
        inline void applyVulnerableToPlayer(int value);
        inline void applyWeak(CharacterData &targetData, int value);
        inline void applyWeakToEnemy(Id targetEntityId, int value);
        inline void applyWeakToPlayer(int value);
        inline void applyFrail(CharacterData &targetData, int value);
        inline void applyFrailToPlayer(int value);
        
        inline void gainVigor(CharacterData &targetData, int value);
        inline void gainVigorEnemy(Id targetEntityId, int value);
        inline void gainVigorPlayer(int value);
        
        inline void gainTempStrength(CharacterData &targetData, int value);
        inline void gainTempStrengthPlayer(int value);
        inline void gainTempDexterity(CharacterData &targetData, int value);
        inline void gainTempDexterityPlayer(int value);



        // Move effects
        EnemyMoveId getNextMove(EnemyMoveId move);
        void advanceMove(EnemyMoveId &move);
        void resolveMove(Id enemyEntityId, Enemy &enemy, EnemyMove &move);
        void executeMove(Id enemyEntityId);
        void executeMoves();



        // Executions
        void executeUnpackCard(Effect &effect);
        void executeFinishCardPlayed(Effect &effect);
        void executeCardDealDamage(Effect &effect);
        void executeCardGainBlock(Effect &effect);
        void executeCardApplyVulnerable(Effect &effect);
        void executeMoveCard(Effect &effect);
        


        // Turn handling
        void reduceValue(int &value, int amount = 1) {
            value -= amount;
            if(value < 0) value = 0;
        }

        void onCharacterBeginTurn(CharacterData &data) {
            data.block = 0;
        }

        void onCharacterEndTurn(CharacterData &data) {
            data.tmpStrength = 0;
            data.tmpDex = 0;
            reduceValue(data.weak);
            reduceValue(data.vulnerable);
            reduceValue(data.frail);
        }

        void advancePhase();
        void handlePhase_PlayerDraw();
        void handlePhase_PlayerUpkeep();
        void handlePhase_Player();
        void handlePhase_PlayerEnd();
        void handlePhase_EnemyUpkeep();
        void handlePhase_Enemy();
        void handlePhase_EnemyEnd();
        void doPhaseActions();
        void endPhase();
        void proceedPhases();



        // Playing cards and working through the stack
        void playCard(Id cardEntityId, Id targetEntityId = TARGET_RANDOM, bool force = false);
        void forcePlayCard(Id cardEntityId, Id targetEntityId = TARGET_RANDOM);
        void resolveStackSingle();
        void resolveStackFully();
        bool isCardPlayable(Card &card);
        bool isCardPlayable(Id cardEntityId);



        // Mics
        void moveCard(Id cardEntityId, CardLocation targetLocation);
        void drawCard();
        void drawCards(int amount);
        void discardById(Id cardEntityId);
        void discardByIndex(int index);
        void discardHand();
        int getRandomNumber(int max);




    public:
        CombatState(EffectPool &effectPool_,
                    CardPool &cardPool_,
                    BuffPool &buffPool,
                    EnemyMovePool &movePool_,
                    EnemyPool &enemyPool_,
                    const Player &player_,
                    const Enemies &enemies_,
                    Cards &cards);
        
        CombatState(const CombatState &other):
                    effectPool(other.effectPool),
                    cardPool(other.cardPool),
                    buffPool(other.buffPool),
                    enemyPool(other.enemyPool),
                    movePool(other.movePool),
                    ecs(other.ecs),
                    variables(other.variables),
                    pileHandler(other.pileHandler),
                    eventRegistry(other.eventRegistry),
                    stack(other.stack),
                    turnId(other.turnId),
                    waitingForActionOnStack(other.waitingForActionOnStack),
                    waitingForAction(other.waitingForAction),
                    stopStack(other.stopStack),
                    combatOver(other.combatOver),
                    actions(other.actions),
                    seedDeck(other.seedDeck),
                    genDeck(other.genDeck)
                    {}

        void setSeeds(unsigned int seedDeck_);
        void increaseSeeds(unsigned int value);

        // Interface methods
        Actions& getActions();
        bool isCombatOver();
        bool isCombatVictorious();
        int getPlayerHealth();
        int getPlayerBlock();
        int getTotalEnemyHp();
        
        // Actions
        void generateActions();
        void executeAction(int actionIndex);
        void startCombat();
        void reshuffleDeck();

        // ToString methods
        std::string stackToString();
        std::string actionsToString();
        std::string toString();
        std::string basicDataToString();

    };

}
