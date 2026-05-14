#pragma once

#include <Core/Character/player_id.hpp>
#include <Core/Character/character_data.hpp>


namespace SpireSim {

    struct Player {
        PlayerId playerId = PlayerId::None;
        CharacterData data;
        // orb slots...

        Player(PlayerId playerId_ = PlayerId::None) : playerId(playerId_) {}
        Player(PlayerId playerId_, int maxHp_) : playerId(playerId_) {
            data.maxHp = maxHp_; data.hp = maxHp_;
        }

        std::string toString() {
            std::stringstream ss;
            ss << "Data: [ " << data.toString() << " ]";
            return ss.str();
        }
    };
    using Players = std::vector<Player>;

}
