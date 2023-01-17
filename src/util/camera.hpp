#ifndef CAMERA_H
#define CAMERA_H

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "../types/transform.hpp"

#include "../global.hpp"
#include "../util/platform.hpp"

struct Camera{
    public:
        float fov = 90.0f;
        float nearClip = 0.01f;
        float farClip = 1000.0f;
        float cameraScale = 5.0f;

        Transform transform{};

        Camera(){}
        Camera(Transform tranform){
            this->transform = tranform;
        }

        glm::mat4 GetPerspectiveMatrix(){
            float width = global.platform->width;
            float height = global.platform->height;

            float aspect = (float)width/height;
            aspect *= cameraScale;
            return glm::ortho(-aspect, aspect, -cameraScale, cameraScale, nearClip, farClip);
        }

        glm::mat4 GetViewMatrix(){
            return glm::lookAt(transform.position, transform.position + transform.Forward(), glm::vec3(0,1,0));
        }

        glm::vec3 ScreenCoordToDirection(glm::vec2 screenCoordinate){
            float width = global.platform->width;
            float height = global.platform->height;
            glm::vec2 normalizedCoords = glm::vec2((2.0f * screenCoordinate.x) / width - 1.0f,
                                                   1.0f - (2.0f * screenCoordinate.y) / height);
            glm::vec4 clipCoords = glm::vec4(normalizedCoords, -1.0f, 1.0f);
            glm::vec4 eyeCoords = glm::inverse(GetPerspectiveMatrix()) * clipCoords;
            glm::vec4 worldCoord = glm::inverse(GetViewMatrix()) * eyeCoords;
            return glm::normalize(glm::vec3(worldCoord));
        }
};

#endif