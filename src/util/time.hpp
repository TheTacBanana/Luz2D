#ifndef TIME_H
#define TIME_H

// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Time{
    public:
        float deltaTime{};
        float lastFrame{}; 

        // Not to be called outside of luz.h
        void CalculateDeltaTime(){
            float currentFrame = GetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }

        float GetFramerate(){
            return 1.0 / deltaTime;
        }

        float GetTime(){
            return static_cast<float>(glfwGetTime());
        }
};

#endif