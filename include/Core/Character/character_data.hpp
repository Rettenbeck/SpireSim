#pragma once

#include <Core/Buff/buff_pool.hpp>


namespace SpireSim {

    int WEAK_VALUE_INITIAL = 25;
    int VULNERABLE_VALUE_INITIAL = 50;
    int FRAIL_VALUE_INITIAL = 25;

    struct CharacterData {
        int hp = 1, maxHp = 1, block = 0;
        int strength = 0, tmpStrength = 0, vigor = 0, dex = 0, tmpDex = 0;
        int weak = 0, vulnerable = 0, frail = 0;

        int weakValue, vulnerableValue, frailValue;
        float weakFactor, vulnerableFactor, frailFactor;

        inline float calculatePercentage(int value) {
            return ((float) value) / 100.0f;
        }

        void recalculateFactors() {
            weakFactor = 1.0f - calculatePercentage(weakValue);
            vulnerableFactor = 1.0f + calculatePercentage(vulnerableValue);
            frailFactor = 1.0f - calculatePercentage(frailValue);
        }

        CharacterData() {
            weakValue = WEAK_VALUE_INITIAL;
            vulnerableValue = VULNERABLE_VALUE_INITIAL;
            frailValue = FRAIL_VALUE_INITIAL;
            recalculateFactors();
        }

        std::string toString() {
            std::stringstream ss;
            ss << "hp: " << hp << "; maxHp: " << maxHp << "; block: " << block;
            ss << "; strength: " << strength << "; tmpStrength: " << tmpStrength << "; vigor: " << vigor;
            ss << "; dex: " << dex << "; tmpDex: " << tmpDex;
            ss << "; weak: " << weak << "; vulnerable: " << vulnerable << "; frail: " << frail;
            return ss.str();
        }
    };

}
