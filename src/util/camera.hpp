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
        float cameraScale = 5.0f;

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
            float aspect = (float)global.platform->width / global.platform->height;
            aspect *= cameraHeight;
            
            return glm::ortho(-aspect, aspect, -cameraHeight, cameraHeight, nearClip, farClip);
        }

        glm::mat4 GetViewMatrix(){
            return glm::lookAt(transform.position, transform.position + transform.Forward(), glm::vec3(0,1,0));
        }
};

#endif