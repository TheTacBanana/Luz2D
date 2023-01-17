#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "../lib/glm/glm.hpp"

#include "../global.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <string>

struct KeyPress{
    int key{};
    int scancode{};
    int action{};
    int mods{};

    int prevAction = GLFW_RELEASE;
};

struct MousePress{
    int button{};
    int action{};
    int mods{};

    int prevAction = GLFW_RELEASE;
};

class InputSystem{
    public:
        std::unordered_map<int, KeyPress> pressedKeys{};
        std::unordered_map<int, MousePress> mouseButtons{};

        glm::vec2 lastMouseAxis{};
        glm::vec2 mouseAxis{};

        float mouseX{};
        float mouseY{};

        float lastX{};
        float lastY{};
        bool firstMouse = true;

        void SetKey(KeyPress kp){
            if (pressedKeys.find(kp.key) != pressedKeys.end()){
                kp.prevAction = pressedKeys[kp.key].action;
            }
            pressedKeys[kp.key] = kp;
        } 

        void SetMouse(MousePress mp){
            if (mouseButtons.find(mp.button) != mouseButtons.end()){
                mp.prevAction = pressedKeys[mp.button].action;
            }
            mouseButtons[mp.button] = mp;
        } 

        // Returns True if pressed or held
        bool GetKey(int keyval){
            int action = pressedKeys[keyval].action;
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                return true;
            return false;
        } 

        // Returns true only on first frame;
        bool GetKeyDown(int keyval){
            if (pressedKeys[keyval].action == GLFW_PRESS && 
                pressedKeys[keyval].prevAction == GLFW_RELEASE){

                pressedKeys[keyval].prevAction = GLFW_REPEAT;
                return true;
            }
            return false;
        }

        // Returns True if pressed or held
        bool GetMouseButton(int mouseval){
            int action = mouseButtons[mouseval].action;
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                return true;
            return false;
        }

        bool GetMouseButtonDown(int mouseval){
            if (mouseButtons[mouseval].action == GLFW_PRESS && 
                mouseButtons[mouseval].prevAction == GLFW_RELEASE){

                mouseButtons[mouseval].prevAction = GLFW_REPEAT;
                return true;
            }
            return false;
        }

        // Get Difference between previous and current mouse position
        glm::vec2 GetMouseAxis(){
            if (firstMouse)
            {
                lastX = mouseAxis.x;
                lastY = mouseAxis.y;
                firstMouse = false;
            }

            mouseX = mouseAxis.x - lastX;
            mouseY = lastY - mouseAxis.y;

            lastX = mouseAxis.x;
            lastY = mouseAxis.y;

            return glm::vec2(mouseX, mouseY);
        }

        // Key Press Callback
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
            global.inputSystem->key_callback_impl(window, key, scancode, action, mods);
        }

        void key_callback_impl(GLFWwindow* window, int key, int scancode, int action, int mods){
            SetKey({key, scancode, action, mods});
        }

        // Mouse Position Callback
        static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
            global.inputSystem->mouse_callback_impl(window, xposIn, yposIn);
        }

        void mouse_callback_impl(GLFWwindow* window, double xposIn, double yposIn){
            mouseAxis = glm::vec2(xposIn, yposIn);
        }

        // Mouse Button Callback
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
            global.inputSystem->mouse_button_callback_impl(window, button, action, mods);
        }

        void mouse_button_callback_impl(GLFWwindow* window, int button, int action, int mods){
            SetMouse({button, action, mods});
        }
};   

#endif