#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "../global.hpp"

#include "../res/resourceloader.hpp"
#include "../gamestate.hpp"
#include "../util/camera.hpp"

#include "../res/shader.hpp"
#include "../res/texture.hpp"

#include "../types/transform.hpp"

struct SpriteRenderer{
    public:
        Shader* spriteShader {nullptr};
        
        SpriteRenderer() = default;

        void Init(){
            global.resourceLoader->LoadResource<Shader>("SpriteShader", "Shaders/sprite.vs", "Shaders/sprite.fs");
            spriteShader = global.resourceLoader->GetResource<Shader>("SpriteShader");

            InitRenderData();
        }

        void DrawSprite(Texture* tex, Transform& transform, glm::vec3 colour = glm::vec3(1.0)){
            spriteShader->use();
            
            float aspect = (float)tex->width / tex->height;
            glm::mat4 prescale = glm::mat4(1.0);
            prescale = glm::scale(prescale, glm::vec3(aspect, 1, 0));

            spriteShader->setMat4("model", transform.GetModelMatrix(prescale));
            spriteShader->setMat4("view", global.gameState->gameCamera->GetViewMatrix());
            spriteShader->setMat4("projection", global.gameState->gameCamera->GetOrthoMatrix());

            spriteShader->setVec3("spriteColour", colour);

            glActiveTexture(GL_TEXTURE0);
            tex->Bind();

            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

    private:
        unsigned int VBO;
        unsigned int quadVAO{};

        void InitRenderData(){
            // configure VAO/VBO
            float vertices[] = { 
                // pos              // tex
                -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
                 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
            
                 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
                 0.5f, -0.5f, 0.0f, 1.0f, 1.0f
            };

            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &VBO);
            
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(quadVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);  
            glBindVertexArray(0);
        }
};

#endif