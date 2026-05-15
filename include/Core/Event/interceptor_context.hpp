#pragma once

#include <Core/Combat/entity.hpp>


namespace SpireSim {

    struct InterceptorContext {
        Id sourceEntityId = ENTITY_NONE;

        InterceptorContext() {}
        InterceptorContext(Id sourceEntityId_) : sourceEntityId(sourceEntityId_) {}

        std::string toString() {
            std::stringstream ss;
            ss << "sourceEntityId: " << sourceEntityId;
            return ss.str();
        }
    };

}
