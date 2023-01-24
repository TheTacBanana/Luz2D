#ifndef ECS_H
#define ECS_H

#include "componentmanager.hpp"
#include "eventmanager.hpp"

using EntityID = std::uint32_t;
struct ECS : ComponentManager<ECS, 
                              EntityID, 
                              EmptyType,
                              EmptyType,
                              128,
                              1024
                              > {
    ECS() = default;

    EventManager renderEvent{};

    void Render(){
        renderEvent.Publish();
    }
};

template <typename C>
struct EntityComponent : EmptyType { 

};


#endif