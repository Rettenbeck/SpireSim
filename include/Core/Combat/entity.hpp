#pragma once

#include <Util/util.hpp>


namespace SpireSim {

    using Id = int;
    using Ids = std::vector<Id>;
    const Id ENTITY_NONE = -1;

    struct Entity {
        Id id;
        Id ownerId;
        
        Entity(Id id_, Id ownerId_ = ENTITY_NONE) : id(id_), ownerId(ownerId_) {}
        
        std::string toString() {
            std::stringstream ss; ss << "id: " << id << "; ownerId: " << ownerId;
            return ss.str();
        }
    };
    using Entities = std::vector<Entity>;

}
