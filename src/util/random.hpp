#ifndef RANDOM_H
#define RANDOM_H

#include "../lib/glm/glm.hpp"
#include <random>

struct Random{
    float GetFloat(float min = 0.0, float max = 1.0){
        return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX/(max - min));
    }
};

#endif