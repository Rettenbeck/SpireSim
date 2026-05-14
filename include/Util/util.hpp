#pragma once

#include <Util/basic.hpp>


namespace SpireSim {

    std::string ToString(int val) { std::stringstream ss; ss << val; return ss.str(); }
    
    template<typename T>
    T FromInt(T type) { return static_cast<T>(type); }

}
