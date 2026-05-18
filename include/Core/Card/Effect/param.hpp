#pragma once

#include <Core/Card/Effect/param_type.hpp>


namespace SpireSim {

    struct Param {
        ParamType paramType;
        int value;
        Param() {}
        Param(const ParamType paramType_) : paramType(paramType_) {}
        Param(const ParamType paramType_, int value_) : paramType(paramType_), value(value_) {}
        
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Param, paramType, value);
        TO_STRING_METHOD
    
        // std::string toString() {
        //     std::stringstream ss;
        //     ss << "paramType: " << ToString(paramType) << "; value: " << value << "; ";
        //     return ss.str();
        // }
    };
    using Params = std::vector<Param>;

}
