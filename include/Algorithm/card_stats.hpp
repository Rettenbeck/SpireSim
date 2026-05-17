#pragma once

#include <Core/Combat/include.hpp>


namespace SpireSim {

    struct CardStats {
        long countPlayable = 0;
        long countPlayedIfAble = 0;
        float countPlayedIfAbleWeighted = 0.f;

        void clear() {
            countPlayable = 0;
            countPlayedIfAble = 0;
            countPlayedIfAbleWeighted = 0.f;
        }

        float getPlayedRate(bool weighted = true) {
            if(weighted) {
                return (countPlayable > 0) ? (((double) countPlayedIfAbleWeighted) / ((double) countPlayable)) : 0;
            } else {
                return (countPlayable > 0) ? (((double) countPlayedIfAble) / ((double) countPlayable)) : 0;
            }
        }

        CardStats& operator+=(const CardStats& other) {
            countPlayable += other.countPlayable;
            countPlayedIfAble += other.countPlayedIfAble;
            countPlayedIfAbleWeighted += other.countPlayedIfAbleWeighted;
            return *this;
        }
    };
    using CardStatsMap = std::map<Id, CardStats>;

    CardStatsMap& Merge(CardStatsMap& left, const CardStatsMap& right) {
        for (const auto& [id, stats] : right) left[id] += stats;
        return left;
    }

    CardStatsMap& Merge(CardStatsMap& target, const std::vector<CardStatsMap> &vec) {
        for(auto& map : vec) Merge(target, map);
        return target;
    }

    std::string ToString(CardStatsMap &cardStatsMap, Combat *initialState = nullptr) {
        if(cardStatsMap.empty()) return "No stats\n";
        
        std::stringstream ss;
        ss << "Card stats:\n";

        for(auto& [cardEntityId, stats] : cardStatsMap) {
            ss << "  Card " << cardEntityId;
                if(initialState != nullptr) ss << " (" << CardIdToString(initialState->getCardIdFromEntityId(cardEntityId)) << ")";
                ss << " seen: " << stats.countPlayable
                << "; played: " << stats.countPlayedIfAble << " (" << stats.getPlayedRate(false) << ")"
                << "; played weighted: " << stats.countPlayedIfAbleWeighted << " (" << stats.getPlayedRate(true) << ")"
                << "\n";
        }
        return ss.str();
    }

}
