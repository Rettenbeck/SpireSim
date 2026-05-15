#pragma once

#include <Core/Relic/relic.hpp>


namespace SpireSim {

    struct RelicPool {
        Relics relics;
        
        RelicPool() {
            createRelicPool();
        }
        
        void createBlankArray(int index) {
            while(index >= relics.size()) {
                relics.push_back(Relic());
            }
        }
        
        Relic& retrieveForCreation(RelicId relicId) {
            int index = int(relicId);
            createBlankArray(index);
            relics[index].relicId = relicId;
            return relics[index];
        }
        
        Relic& retrieve(RelicId relicId) {
            int index = int(relicId);
            assert(index < relics.size());
            return relics[index];
        }
        
        void createRelicPool() {
            createBlankArray(int(RelicId::Count));
            auto& relicBag = retrieveForCreation(RelicId::BagOfPreparation);
            auto& relicPuzzle = retrieveForCreation(RelicId::CentennialPuzzle);

            relicBag.eventList.push_back({EventType::AfterFirstDrawPhase, EventListener(
                Effect(EffectType::DrawCards, {Param(ParamType::FixedValue, 2)}
            ))});

            relicPuzzle.eventList.push_back({EventType::OnFirstDamageTaken, EventListener(
                Effect(EffectType::DrawCards, {Param(ParamType::FixedValue, 3)}
            ))});
        }
        
        std::string toString() {
            std::stringstream ss;
            ss << "RelicPool: { ";
            for(auto& relic : relics) ss << "[ " << relic.toString() << "] ";
            ss << "};";
            return ss.str();
        }
    };

}
