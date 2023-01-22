#ifndef PLATFORM_H
#define PLATFORM_H

// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../global.hpp"

#include "../input/inputsystem.hpp"
#include "time.hpp"

enum DisplayMode{
    Fullscreen = 0,
    Windowed = 1,
    WindowedBorderless = 2
};

struct Platform {
    public:
        unsigned int width = 1000, height = 800;
        GLFWwindow* window{nullptr};
        DisplayMode mode {Windowed};

        Platform(){
            InitGLFW(); 
            InitOpenGL();
        }
        
        void SetDisplayMode(DisplayMode mode){
            this->mode = mode;
            if (mode == Fullscreen){
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            } else if (mode == Windowed){
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                int xpos, ypos;
                CenterWindowToMonitor(monitor, xpos, ypos);
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(window, NULL, xpos, ypos, width, height, mode->refreshRate);
            } else if (mode == WindowedBorderless){
                // TODO
            }
        }

        void CenterWindowToMonitor(GLFWmonitor* monitor, int& xpos, int& ypos){
            int xposnew, yposnew;
            glfwGetMonitorPos(monitor, &xposnew, &yposnew);

            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            int monitorWidth = mode->width, monitorHeight = mode->height;

            xpos = xposnew + (monitorWidth / 2.0) - (width / 2.0);
            ypos = yposnew + (monitorHeight / 2.0) - (height / 2.0);
        }

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
            global.platform->framebuffer_size_callback_impl(window, width, height);
        }

        void framebuffer_size_callback_impl(GLFWwindow* window, int width, int height){
            glViewport(0, 0, width, height);
            this->width = width;
            this->height = height;
        }

        void StartFrame(){
             // Calculate Delta Time
            global.time->CalculateDeltaTime();

            // Clear Backround
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void EndFrame(){
            glfwSwapBuffers(global.platform->window);
            glfwPollEvents();
        }

        void Terminate(){
            glfwSetWindowShouldClose(window, true);
        }

        void Cleanup(){
            glfwTerminate();
        }

        bool ShouldClose(){
            return glfwWindowShouldClose(window);
        }

    private:
        void InitGLFW(){
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(width, height, "", NULL, NULL);
            if (window == NULL) {
                std::cout << "[ Failed to create GLFW window ]" << std::endl;
                glfwTerminate();
            }

            glfwMakeContextCurrent(window);

            glfwSetFramebufferSizeCallback(window, Platform::framebuffer_size_callback);

            // Key & Mouse Callback
            glfwSetKeyCallback(window, InputSystem::key_callback);
            glfwSetCursorPosCallback(window, InputSystem::mouse_callback);
            glfwSetMouseButtonCallback(window, InputSystem::mouse_button_callback);
        }

        void InitOpenGL(){
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cout << "[ Failed to initialize GLAD ]" << std::endl;
                exit(0);
            }

            glEnable(GL_DEPTH_TEST);
            //glEnable(GL_CULL_FACE);

            glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
        }
};

#endif