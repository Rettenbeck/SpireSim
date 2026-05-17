#pragma once

#include <Core/Buff/buff_pool.hpp>
#include <Core/Character/Move/next_move_list.hpp>

namespace SpireSim {

    struct EnemyMove {
        BuffPool &buffPool;
        EnemyMoveId enemyMoveId = EnemyMoveId::None;
        int damage = 0, attacks = 0, block = 0;
        int strengthGain = 0, vigor = 0;
        int applyWeak = 0, applyFrail = 0, applyVulnerable = 0;
        Buffs buffsToGain;
        Buffs debuffsToApply;

        EnemyMove(BuffPool &buffPool_) : buffPool(buffPool_) {}

        EnemyMove& addAttack(int damage_, int attacks_ = 1) {
            damage = damage_;
            attacks = attacks_;
            return *this;
        }

        EnemyMove& addBlock(int block_) {
            block = block_; return *this;
        }

        EnemyMove& gainStrength(int value) {
            strengthGain = value; return *this;
        }

        EnemyMove& addDebuff(const Buff &buff) {
            debuffsToApply.push_back(buff); return *this;
        }

        EnemyMove& addDebuff(BuffId buffId) {
            addDebuff(buffPool.createBuffFromTemplate(buffId)); return *this;
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
