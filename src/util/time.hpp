#ifndef TIME_H
#define TIME_H

// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../global.hpp"
#include "../gamestate.hpp"
#include "../compsys/ecs.hpp"

struct Time{
    public:
        const float globalTickRate = 60.0;
        float tickTimer{};

        float deltaTime{};
        float lastFrame{}; 

        // Not to be called outside of platform.hpp
        void CalculateDeltaTime(){
            float currentFrame = GetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Global Tick Update Call
            float timePerTick = 1 / globalTickRate;
            tickTimer += deltaTime;
            if (tickTimer >= timePerTick){
                tickTimer -= timePerTick;
                global.gameState->ecs->TickUpdate();
            }
        }

        float GetFramerate(){
            return 1.0 / deltaTime;
        }

        float GetTime(){
            return static_cast<float>(glfwGetTime());
        }
};

#endif