#pragma once

#include <Core/Card/Modifiers/enchantment_type.hpp>


namespace SpireSim {

    struct Enchantment {
        EnchantmentType enchantmentType;
        int value;
        
        Enchantment() : enchantmentType(EnchantmentType::None), value(0) {}
        Enchantment(EnchantmentType enchantmentType_) : enchantmentType(enchantmentType_), value(0) {}
        Enchantment(EnchantmentType enchantmentType_, int value_) : enchantmentType(enchantmentType_), value(value_) {}
        Enchantment(const Enchantment &enchantment_) : enchantmentType(enchantment_.enchantmentType), value(enchantment_.value) {}

        std::string toString() {
            std::stringstream ss; ss << "[enchantmentType: " << ToString(enchantmentType) << "; value: " << value << "]";
            return ss.str();
        }
    };

}
