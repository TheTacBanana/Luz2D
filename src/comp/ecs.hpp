#ifndef ECS_H
#define ECS_H

#include "componentmanager.hpp"

template <typename C>
struct EntityComponent : EmptyType { };
struct ECSCompTypeBase { };

using EntityID = std::uint32_t;
struct ECS : ComponentManager<ECS, 
                              EntityID, 
                              EmptyType,
                              EmptyType,
                              128,
                              1024
                              > {
    ECS() = default;
};


#endif