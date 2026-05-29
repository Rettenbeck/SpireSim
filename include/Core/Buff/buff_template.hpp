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

        BuffTemplate& dependOnCreator(bool dependentOnCreator_ = true) {
            dependentOnCreator = dependentOnCreator_;
            return *this;
        }

        BuffTemplate& modifyParentDamagePerc(const Param &param) {
            eventList.push_back({
                EventType::OnDealDamageForInterception,
                EventListener(Effect(EffectType::ModifyParentDamagePerc, {param}))
            });
            return *this;
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(BuffTemplate, buffId, buffCategory, amountParams, dependentOnCreator, eventList);
        TO_STRING_METHOD
    
    };
    using BuffTemplates = std::vector<BuffTemplate>;

}
