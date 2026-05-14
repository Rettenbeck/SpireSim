#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    enum class ConditionType {
        Equal, LowerThan, LowerEqual, GreaterThan, GreaterEqual, DivisibleBy
    };
    std::string ToString(ConditionType e) { return ToString(int(e)); }

}
