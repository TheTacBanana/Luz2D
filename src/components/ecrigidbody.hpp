#ifndef ECRIGIDBODY_H
#define ECRIGIDBODY_H

#include "../compsys/ecs.hpp"
#include "../gamestate.hpp"
#include "../physics/physicsmanager.hpp"
#include "../lib/glm/glm.hpp"
#include "../util/time.hpp"
#include "ectransform.hpp"
#include "../global.hpp"

struct ECRigidbody : EntityComponent {
    glm::vec3 gravity = glm::vec3(0, -9.8, 0);
    
    bool useGravity{true};

    glm::vec3 velocity{};
    glm::vec3 acceleration{};
    float mass{ 1.0 };

    glm::vec3 prevPosition {};

    ECRigidbody(bool useGravity = true){
        this->useGravity = useGravity;
    }

    void Init(){
        global.gameState->ecs->physicsObjects.push_back(index);

        prevPosition = base->GetComponent<ECTransform>(index)->transform.position;
    }

    void PhysicsUpdate(){
        if (useGravity)
            acceleration = mass * gravity;
        velocity += acceleration / mass * global.time->deltaTime;
        prevPosition = base->GetComponent<ECTransform>(index)->transform.position;
        base->GetComponent<ECTransform>(index)->transform.position += velocity * global.time->deltaTime;
        acceleration = glm::vec3(0);
    }
};

#endif