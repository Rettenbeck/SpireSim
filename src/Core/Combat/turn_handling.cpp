#pragma once

#include <Core/Combat/combat_state.hpp>


namespace SpireSim {

    void CombatState::advancePhase() {
        switch (turnId) {
            case TurnId::None        : turnId = TurnId::PlayerDraw  ; return;
            case TurnId::PlayerDraw  : turnId = TurnId::PlayerUpkeep; return;
            case TurnId::PlayerUpkeep: turnId = TurnId::Player      ; return;
            case TurnId::Player      : turnId = TurnId::PlayerEnd   ; return;
            case TurnId::PlayerEnd   : turnId = TurnId::EnemyUpkeep ; return;
            case TurnId::EnemyUpkeep : turnId = TurnId::Enemy       ; return;
            case TurnId::Enemy       : turnId = TurnId::EnemyEnd    ; return;
            case TurnId::EnemyEnd    : turnId = TurnId::PlayerDraw  ; return;
            case TurnId::Count       : turnId = TurnId::PlayerDraw  ; return;
            default: assert(false);
        }
    }

    void CombatState::handlePhase_PlayerDraw() {
        drawCards(variables.cardDrawOnTurn);
    }
    
    void CombatState::handlePhase_PlayerUpkeep() {
        variables.turn++;
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
