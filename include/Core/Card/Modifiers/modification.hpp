#pragma once

#include <Core/Card/Modifiers/modification_type.hpp>


namespace SpireSim {

    struct Modification {
        ModificationType modificationType;
        int value;
        
        Modification(ModificationType modificationType_) : modificationType(modificationType_) {}
        Modification(ModificationType modificationType_, int value_) : modificationType(modificationType_), value(value_) {}
        
        std::string toString() {
            std::stringstream ss; ss << "[modificationType: " << ToString(modificationType) << "; value: " << value << "]";
            return ss.str();
        }
    };
    using Modifications = std::vector<Modification>;

}
