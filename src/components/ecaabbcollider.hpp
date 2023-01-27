#ifndef AABB_COLLIDER_H
#define AABB_COLLIDER_H

#include "../compsys/ecs.hpp"
#include "ectransform.hpp"
#include "../lib/glm/glm.hpp"
#include "../global.hpp"

struct ECBoxCollider : EntityComponent {
    float dx { 0.5 };
    float dy { 0.5 };

    ECBoxCollider() = default;
    ECBoxCollider(float dx, float dy){
        this->dx = dx;
        this->dy = dy;
    }

    bool Intersect(ECBoxCollider* other){
        glm::vec3 pos      = base->GetComponent<ECTransform>(index)->transform.position;
        glm::vec3 otherpos = base->GetComponent<ECTransform>(other->index)->transform.position;
        return ((pos.x - dx) <= (otherpos.x + other->dx) &&
                (pos.x + dx) >= (otherpos.x - other->dx) &&
                (pos.y - dy) <= (otherpos.y + other->dy) &&
                (pos.y + dy) >= (otherpos.y - other->dy));
    }
};

#endif