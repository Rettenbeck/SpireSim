#pragma once

#include <Core/Card/Effect/condition_type.hpp>
#include <Core/Card/Effect/param.hpp>


namespace SpireSim {

    struct Condition {
        ConditionType conditionType;
        Param param1, param2;
        
        Condition() {}
        Condition(ConditionType conditionType_) : conditionType(conditionType_) {}
        Condition(ConditionType conditionType_, const Param &param1_, const Param &param2_) : conditionType(conditionType_), param1(param1_), param2(param2_) {}
        
        bool evaluate(int value1, int value2) {
            switch(conditionType) {
                case ConditionType::Equal       : return (value1 == value2);
                case ConditionType::LowerThan   : return (value1 <  value2);
                case ConditionType::LowerEqual  : return (value1 <= value2);
                case ConditionType::GreaterThan : return (value1 >  value2);
                case ConditionType::GreaterEqual: return (value1 >= value2);
                case ConditionType::DivisibleBy : return (value1 %  value2 == 0);
                default: assert(false); return false;
            }
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "condition: " << ToString(conditionType) << "; param1: " << param1.toString() << "; param2: " << param2.toString();
            return ss.str();
        }
    };
    using Conditions = std::vector<Condition>;

}
