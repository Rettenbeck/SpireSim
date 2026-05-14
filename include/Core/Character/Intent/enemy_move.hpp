#pragma once

#include <Core/Buff/buff.hpp>
#include <Core/Character/Intent/enemy_move_id.hpp>

namespace SpireSim {

    struct EnemyMove {
        EnemyMoveId enemyMoveId;
        int damage = 0, attacks = 0, block = 0;
        int strengthGain = 0, vigor = 0;
        int applyWeak = 0, applyFrail = 0, applyVulnerable = 0;
        Buffs buffsToGain;
        Buffs debuffsToApply;

        EnemyMove() : enemyMoveId(EnemyMoveId::None) {}
        EnemyMove(EnemyMoveId enemyMoveId_) : enemyMoveId(enemyMoveId_) {}

        inline void addAttack(int damage_, int attacks_ = 1) {
            damage = damage_;
            attacks = attacks_;
        }

        std::string toString() {
            std::stringstream ss;
            ss << "enemyMoveId: " << ToString(enemyMoveId) << "; Damage: " << damage << "; Attacks: " << attacks
                << "; Block: " << block << "; strengthGain: " << strengthGain
                << "; applyWeak: " << applyWeak << "; applyFrail: " << applyFrail << "; applyVulnerable: " << applyVulnerable;
            ss << "; BuffsToGain: { ";
            for(auto& buff : buffsToGain) ss << "[ " << buff.toString() << "] ";
            ss << "}";
            ss << "; DebuffsToApply: { ";
            for(auto& buff : debuffsToApply) ss << "[ " << buff.toString() << "] ";
            ss << "}";
            return ss.str();
        }
    };
    using EnemyMoves = std::vector<EnemyMove>;

}
