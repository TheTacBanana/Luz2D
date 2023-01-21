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

        void DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
        {
            // prepare transformations
            spriteShader->use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

            model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
            model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
            model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

            model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

            spriteShader->setMat4("model", model);

            glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(global.platform->width), 
                                        static_cast<float>(global.platform->height), 0.0f, -1.0f, 1.0f);

            spriteShader->setMat4("projection", projection);

            // render textured quad
            spriteShader->setVec3("spriteColor", color);

            glActiveTexture(GL_TEXTURE0);
            texture->Bind();

            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

    private:
        unsigned int quadVAO{};

        void InitRenderData(){
            // configure VAO/VBO
            unsigned int VBO;
            float vertices[] = { 
                // pos      // tex
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 
            
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &VBO);
            
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(quadVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);  
            glBindVertexArray(0);
        }
};

#endif