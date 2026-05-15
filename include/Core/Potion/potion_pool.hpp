#pragma once

#include <Core/Potion/potion.hpp>


namespace SpireSim {

    struct PotionPool {
        Potions potions;
        
        PotionPool() {
            createPotionPool();
        }
        
        void createBlankArray(int index) {
            while(index >= potions.size()) {
                potions.push_back(Potion());
            }
        }
        
        Potion& retrieveForCreation(PotionId potionId) {
            int index = int(potionId);
            createBlankArray(index);
            potions[index].potionId = potionId;
            return potions[index];
        }
        
        Potion& retrieve(PotionId potionId) {
            int index = int(potionId);
            assert(index < potions.size());
            return potions[index];
        }
        
        void createPotionPool() {
            createBlankArray(int(PotionId::Count));
            auto& potionFire = retrieveForCreation(PotionId::FirePotion);
            auto& potionFlex = retrieveForCreation(PotionId::FlexPotion);
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "PotionPool: { ";
            for(auto& potion : potions) ss << "[ " << potion.toString() << "] ";
            ss << "};";
            return ss.str();
        }
    };

}
