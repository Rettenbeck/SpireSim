#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class EnchantmentType {
        None, Sharp, Nimble
    };
    std::string ToString(EnchantmentType e) { return ToString(int(e)); }

}
