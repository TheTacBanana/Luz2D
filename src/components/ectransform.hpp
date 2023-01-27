#ifndef ECTRANSFORM_H
#define ECTRANSFORM_H

#include "../compsys/ecs.hpp"
#include "../types/transform.hpp"

struct ECTransform : EntityComponent {
    Transform transform{};

    ECTransform() = default;
    ECTransform(glm::vec3 pos){
        transform.position = pos;
    }
};

#endif