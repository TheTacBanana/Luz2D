#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "../compsys/ecs.hpp"
#include "../components/ecrigidbody.hpp"
#include "../components/ecaabbcollider.hpp"
#include "../components/ectransform.hpp"
#include "../global.hpp"

struct PhysicsManager{
    int resolveSteps = 16;

    void PhysicsUpdate(){
        for (auto index1 : global.gameState->ecs->physicsObjects){
            auto obj1 = global.gameState->ecs->GetObject(index1);
            auto rb1 = obj1->GetComponent<ECRigidbody>();
            auto trans1 = obj1->GetComponent<ECTransform>();

            rb1->PhysicsUpdate();

            auto aabb1 = obj1->GetComponent<ECBoxCollider>();
            if (aabb1 == nullptr) continue;

            for (auto index2 : global.gameState->ecs->physicsObjects){
                if (index1 == index2) break;

                auto obj2 = global.gameState->ecs->GetObject(index2);
                auto rb2 = obj2->GetComponent<ECRigidbody>();
                auto aabb2 = obj2->GetComponent<ECBoxCollider>();
                auto trans2 = obj2->GetComponent<ECTransform>();

                if (aabb2 == nullptr) continue;

                if (aabb1->Intersect(aabb2)){
                    glm::vec3 obj1dir = trans1->transform.position - rb1->prevPosition;
                    glm::vec3 obj2dir = trans2->transform.position - rb2->prevPosition;

                    // Between 0 and 1
                    float bestSolution = 0.0;
                    float maxTested = 1.0;  

                    for (int i = 0; i < resolveSteps; i++){
                        float tryThis = (bestSolution + maxTested) / 2.0;

                        trans1->transform.position = rb1->prevPosition + obj1dir * tryThis;
                        trans2->transform.position = rb2->prevPosition + obj2dir * tryThis;

                        if (aabb1->Intersect(aabb2)){ // Failed
                            maxTested = tryThis;
                        } else {                      // Success
                            bestSolution = tryThis;
                        }
                    }
    
                    trans1->transform.position = rb1->prevPosition + obj1dir * bestSolution;
                    trans2->transform.position = rb2->prevPosition + obj2dir * bestSolution;
                    rb1->velocity = glm::vec3(0.0);
                    rb2->velocity = glm::vec3(0.0);
                }
            }
        }
    }
};

#endif