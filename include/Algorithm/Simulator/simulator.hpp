#pragma once

#include <Algorithm/Simulator/combat_template.hpp>
#include <Algorithm/implementor.hpp>


namespace SpireSim {

    class Simulator {
    public:
    
        UImplementor implementor = nullptr;
        UCombatTemplates templates;

        Simulator() {}
        Simulator(UImplementor implementor_) : implementor(std::move(implementor_)) {}
        
        //
        
    };

}
