#pragma once

#include <Core/Combat/combat.hpp>


namespace SpireSim {

    void Combat::reduceValue(int &value, int amount) {
        value -= amount;
        if(value < 0) value = 0;
    }

    void Combat::onCharacterBeginTurn(CharacterData &data) {
        data.block = 0;
    }

    void Combat::onCharacterEndTurn(CharacterData &data) {
        data.tmpStrength = 0;
        data.tmpDex = 0;
        reduceValue(data.weak);
        reduceValue(data.vulnerable);
        reduceValue(data.frail);
    }

    void Combat::advancePhase() {
        auto& tId = state.turnId;
        switch (tId) {
            case TurnId::None        : tId = TurnId::CombatStart ; return;
            case TurnId::CombatStart : tId = TurnId::PlayerPre   ; return;
            case TurnId::PlayerPre   : tId = TurnId::PlayerDraw  ; return;
            case TurnId::PlayerDraw  : tId = TurnId::PlayerUpkeep; return;
            case TurnId::PlayerUpkeep: tId = TurnId::Player      ; return;
            case TurnId::Player      : tId = TurnId::PlayerEnd   ; return;
            case TurnId::PlayerEnd   : tId = TurnId::EnemyUpkeep ; return;
            case TurnId::EnemyUpkeep : tId = TurnId::Enemy       ; return;
            case TurnId::Enemy       : tId = TurnId::EnemyEnd    ; return;
            case TurnId::EnemyEnd    : tId = TurnId::PlayerPre   ; return;
            case TurnId::Count       : tId = TurnId::PlayerPre   ; return;
            default: assert(false);
        }
    }

    void Combat::handlePhase_CombatStart() {
        //
    }
    
    void Combat::handlePhase_PlayerPre() {
        variables.turn++;
    }
    
    void Combat::handlePhase_PlayerDraw() {
        drawCards(variables.cardDrawOnTurn);
        if(variables.turn == 1) triggerEvent(EventType::AfterFirstDrawPhase);
    }
    
    void Combat::handlePhase_PlayerUpkeep() {
        variables.energy = variables.energyPerTurn;
        onCharacterBeginTurn(ecs.getPlayer().data);
    }
    
    void Combat::handlePhase_Player() {
        generateActions();
        state.waitingForAction = true;
    }
    
    void Combat::handlePhase_PlayerEnd() {
        discardHand();
        onCharacterEndTurn(ecs.getPlayer().data);
    }
    
    void Combat::handlePhase_EnemyUpkeep() {
        for(auto enemyId : ecs.enemyEntityIds)
            onCharacterBeginTurn(ecs.getEnemy(enemyId).data);
    }
    
    void Combat::handlePhase_Enemy() {
        executeMoves();
    }
    
    void Combat::handlePhase_EnemyEnd() {
        for(auto enemyId : ecs.enemyEntityIds)
            onCharacterEndTurn(ecs.getEnemy(enemyId).data);
    }
    
    void Combat::doPhaseActions() {
        switch (state.turnId) {
            case TurnId::CombatStart : handlePhase_CombatStart (); return;
            case TurnId::PlayerPre   : handlePhase_PlayerPre   (); return;
            case TurnId::PlayerDraw  : handlePhase_PlayerDraw  (); return;
            case TurnId::PlayerUpkeep: handlePhase_PlayerUpkeep(); return;
            case TurnId::Player      : handlePhase_Player      (); return;
            case TurnId::PlayerEnd   : handlePhase_PlayerEnd   (); return;
            case TurnId::EnemyUpkeep : handlePhase_Enemy       (); return;
            case TurnId::Enemy       : handlePhase_EnemyUpkeep (); return;
            case TurnId::EnemyEnd    : handlePhase_EnemyEnd    (); return;
            default: assert(false);
        }
    }

    void Combat::endPhase() {
        advancePhase();
        doPhaseActions();
    }

    void Combat::proceedPhases() {
        while(!state.waitingForAction) {
            endPhase();
        }
    }

}
