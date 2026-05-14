#pragma once

#include <Core/Character/player.hpp>


namespace SpireSim {

    struct PlayerFactory {
        PlayerFactory() {}

        Player get(PlayerId playerId) {
            switch(playerId) {
                case PlayerId::IronClad   : return Player(playerId, 80);
                case PlayerId::Silent     : return Player(playerId, 70);
                case PlayerId::Regent     : return Player(playerId, 75);
                case PlayerId::NecroBinder: return Player(playerId, 66);
                case PlayerId::Defect     : return Player(playerId, 75);
                default: assert(false);
            }
        }
    };

}
