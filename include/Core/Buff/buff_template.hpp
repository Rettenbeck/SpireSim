#pragma once

#include <Core/Buff/buff.hpp>
#include <Core/Buff/buff_category.hpp>


namespace SpireSim {

    struct BuffTemplate {
        BuffId buffId = BuffId::None;
        BuffCategory buffCategory = BuffCategory::None;
        int amountParams;
        bool dependentOnCreator;

        EventList eventList;

        BuffTemplate(int amountParams_ = 0, bool dependentOnCreator_ = false)
            : amountParams(amountParams_), dependentOnCreator(dependentOnCreator_) {}

        std::string toString() {
            std::stringstream ss;
            ss << "BuffId: " << ToString(buffId) << "BuffCat: " << ToString(buffCategory) << "; amountParams: " << amountParams
                 << "; dependentOnCreator: " << dependentOnCreator;
            return ss.str();
        }
    };
    using BuffTemplates = std::vector<BuffTemplate>;

}
