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

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(BuffTemplate, buffId, buffCategory, amountParams, dependentOnCreator, eventList);
        TO_STRING_METHOD
    
    };
    using BuffTemplates = std::vector<BuffTemplate>;

}
