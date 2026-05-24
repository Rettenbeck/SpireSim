#pragma once

#include <Core/Buff/buff_template.hpp>
#include <Core/constants.hpp>


namespace SpireSim {

    struct BuffPool {
        BuffTemplates buffTemplates;
        
        BuffPool() {
            createBuffPool();
        }
        
        void createBlankArray(int index) {
            while(index >= buffTemplates.size()) {
                buffTemplates.push_back(BuffTemplate());
            }
        }
        
        BuffTemplate& retrieveForCreation(BuffId buffId) {
            int index = int(buffId);
            createBlankArray(index);
            assert(buffTemplates.size() > index);
            buffTemplates[index].buffId = buffId;
            return buffTemplates[index];
        }
        
        BuffTemplate& retrieveForCreation(BuffId buffId, BuffCategory buffCategory, int amountParams = 0) {
            auto& buffTemplate = retrieveForCreation(buffId);
            buffTemplate.buffCategory = buffCategory;
            buffTemplate.amountParams = amountParams;
            return buffTemplate;
        }
        
        BuffTemplate& retrieve(BuffId buffId) {
            int index = int(buffId);
            assert(index < buffTemplates.size());
            return buffTemplates[index];
        }
        
        Buff createBuffFromTemplate(BuffId buffId) {
            Buff buff;
            retrieveForCreation(buffId);
            buff.buffId = buffId;
            return buff;
        }
        
        Buff createBuffFromTemplate(BuffId buffId, int param) {
            auto buff = createBuffFromTemplate(buffId);
            buff.params[0] = param;
            return buff;
        }
        
        Buff createBuffFromTemplate(BuffId buffId, int param1, int param2) {
            auto buff = createBuffFromTemplate(buffId);
            buff.params[0] = param1;
            buff.params[1] = param2;
            return buff;
        }
        
        void createBuffPool() {
            createBlankArray(int(BuffId::Count));
            
            retrieveForCreation(BuffId::GainStrengthPerTurn, BuffCategory::Buff, 1);

            retrieveForCreation(BuffId::Shrink, BuffCategory::Debuff)
                .modifyParentDamagePerc(Param(ParamType::FixedValue, BUFF_SHRINKER_PERCENTAGE))
                .dependOnCreator();
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "BuffPool: { ";
            for(auto buffTemplate : buffTemplates) ss << "[ " << buffTemplate.toString() << "] ";
            ss << "};";
            return ss.str();
        }
    };

}
