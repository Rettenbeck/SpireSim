#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::reduceValue(int &value, int amount) {
        value -= amount;
        if(value < 0) value = 0;
    }

    void CombatState::onCharacterBeginTurn(CharacterData &data) {
        data.block = 0;
    }

    void CombatState::onCharacterEndTurn(CharacterData &data) {
        data.tmpStrength = 0;
        data.tmpDex = 0;
        reduceValue(data.weak);
        reduceValue(data.vulnerable);
        reduceValue(data.frail);
    }

    void CombatState::advancePhase() {
        switch (turnId) {
            case TurnId::None        : turnId = TurnId::CombatStart ; return;
            case TurnId::CombatStart : turnId = TurnId::PlayerPre   ; return;
            case TurnId::PlayerPre   : turnId = TurnId::PlayerDraw  ; return;
            case TurnId::PlayerDraw  : turnId = TurnId::PlayerUpkeep; return;
            case TurnId::PlayerUpkeep: turnId = TurnId::Player      ; return;
            case TurnId::Player      : turnId = TurnId::PlayerEnd   ; return;
            case TurnId::PlayerEnd   : turnId = TurnId::EnemyUpkeep ; return;
            case TurnId::EnemyUpkeep : turnId = TurnId::Enemy       ; return;
            case TurnId::Enemy       : turnId = TurnId::EnemyEnd    ; return;
            case TurnId::EnemyEnd    : turnId = TurnId::PlayerPre   ; return;
            case TurnId::Count       : turnId = TurnId::PlayerPre   ; return;
            default: assert(false);
        }
    }

    void CombatState::handlePhase_CombatStart() {
        //
    }
    
    void CombatState::handlePhase_PlayerPre() {
        variables.turn++;
    }
    
    void CombatState::handlePhase_PlayerDraw() {
        drawCards(variables.cardDrawOnTurn);
        if(variables.turn == 1) triggerEvent(EventType::AfterFirstDrawPhase);
    }
    
    void CombatState::handlePhase_PlayerUpkeep() {
        variables.energy = variables.energyPerTurn;
        onCharacterBeginTurn(ecs.getPlayer().data);
    }
    
    void CombatState::handlePhase_Player() {
        generateActions();
        waitingForAction = true;
    }
    
    void CombatState::handlePhase_PlayerEnd() {
        discardHand();
        onCharacterEndTurn(ecs.getPlayer().data);
    }
    
    void CombatState::handlePhase_EnemyUpkeep() {
        for(auto enemyId : ecs.enemyEntityIds)
            onCharacterBeginTurn(ecs.getEnemy(enemyId).data);
    }
    
    void CombatState::handlePhase_Enemy() {
        executeMoves();
    }
    
    void CombatState::handlePhase_EnemyEnd() {
        for(auto enemyId : ecs.enemyEntityIds)
            onCharacterEndTurn(ecs.getEnemy(enemyId).data);
    }
    
    void CombatState::doPhaseActions() {
        switch (turnId) {
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

    void CombatState::endPhase() {
        advancePhase();
        doPhaseActions();
    }

    void CombatState::proceedPhases() {
        while(!waitingForAction) {
            endPhase();
        }
    }

}
