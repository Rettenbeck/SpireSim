#pragma once

#include <Algorithm/Simulator/combat_template.hpp>


namespace SpireSim {

    struct CombatTemplateFixed : public CombatTemplate {

        CombatTemplateFixed(PoolContainer &container_) : CombatTemplate(container_) {}
        
        void convert() {
            combats.clear();
            combats.push_back(
                std::make_unique<SpireSim::Combat>(
                    *container.effectPool, *container.relicPool, *container.potionPool,
                    *container.cardPool, *container.buffPool, *container.movePool, *container.enemyPool,
                    player, enemies, relics, potions, deck
                )
            );
        }

        Combat* get() {
            if(combats.empty()) convert();
            assert(combats.size() <= 1);
            return combats[0].get();
        }

    };

}
