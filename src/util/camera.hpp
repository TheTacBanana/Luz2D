#ifndef CAMERA_H
#define CAMERA_H

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "../types/transform.hpp"

#include "../global.hpp"
#include "../util/platform.hpp"

#include "../lib/glm/gtx/string_cast.hpp"

struct Camera{
    public:
        float nearClip = 0.0f;
        float farClip = 100.0f;
        float cameraHeight = 5.0f;

        Transform transform{};

        Camera(){
            transform.rotation.y = -90; 
        }
        Camera(Transform transform){
            this->transform = transform;
            transform.rotation.y = -90;
        }

        glm::mat4 GetOrthoMatrix(){
            float halfHeight = cameraHeight / 2.0;
            float aspect = GetAspect();
            return glm::ortho(-aspect, aspect, -halfHeight, halfHeight, nearClip, farClip);
        }

        float GetAspect(){
            float aspect = (float)global.platform->width / global.platform->height;
            float halfHeight = cameraHeight / 2.0;
            aspect *= halfHeight;
            return aspect;
        }

        glm::mat4 GetViewMatrix(){
            return glm::lookAt(transform.position, transform.position + transform.Forward(), glm::vec3(0,1,0));
        }

        glm::vec3 ScreenPositionToWorldSpace(glm::vec2 pos){
            float width = global.platform->width;
            float height = global.platform->height;

            glm::vec3 camCenter = transform.position;

            float halfHeight = cameraHeight / 2.0;
            float aspect = GetAspect();

            float xpos = (pos.x / width) * aspect * 2 - aspect;
            float ypos = (pos.y / height) * halfHeight * 2 - halfHeight;

            return glm::vec3(camCenter.x + xpos, camCenter.y - ypos, 0);
        }   
};

#endif