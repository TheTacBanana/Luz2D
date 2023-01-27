#ifndef ECS_H
#define ECS_H

#include "componentmanager.hpp"
#include "eventmanager.hpp"

template <typename C, // Base Type 
          typename I // Index Type
          >
struct ComponentBase{
    C* base;
    I index;  

    virtual void Init(){};
};

using EntityID = std::uint32_t;
struct ECS : ComponentManager<ECS,
                              EntityID,
                              EmptyType,
                              ComponentBase<ECS, EntityID>,
                              128,
                              1024
                              > {
    ECS() = default;

    void SetupComponent(ComponentBase<ECS, EntityID>* comp, EntityID index){
        comp->base = this;
        comp->index = index;
        comp->Init();
    }

    // Update Events
    EventManager updateEvent{};
    EventManager tickUpdateEvent{};
    EventManager renderEvent{};

    void Update(){
        updateEvent.Publish();
    }

    void TickUpdate(){
        tickUpdateEvent.Publish();
    }

    void Render(){
        renderEvent.Publish();
    }

    // Physics
    std::vector<EntityID> physicsObjects{};
};

struct EntityComponent : ComponentBase<ECS, EntityID> { };

#endif