#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.hpp"
#endif

#include <glad/glad.h>
#include "../lib/glm/glm.hpp"
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "../res/texture.hpp"

struct Sprite{
    glm::vec2 coordStart;
    glm::vec2 coordEnd;
};

struct SpriteSheet{
    public:
        SpriteSheet(Texture* tex){
            this->tex = tex;
        }

        void FixedSlice(int dx, int dy){
            int hSlices = floor((float)tex->Width() / dx);
            int vSlices = floor((float)tex->Height() / dy);

            for (size_t row = 0; row < vSlices; row++){
                for (size_t col = 0; col < hSlices; col++){
                    SliceSpriteSheet("sprite" + std::to_string(insertionOrder.size()),
                                     glm::ivec2(col * dx, row * dy),
                                     glm::ivec2(dx, dy));
                }
            }
        }

        //                                                       (Top Left)
        void SliceSpriteSheet(std::string identifier, glm::ivec2 coordStart, glm::ivec2 size){
            auto width = tex->Width();
            auto height = tex->Height();

            //         (Bottom Right)
            glm::ivec2 coordEnd = coordStart + size;

            glm::vec2 startFloat = glm::vec2((float)coordStart.x / width, (float)coordStart.y / height);
            glm::vec2 endFloat = glm::vec2((float)coordEnd.x / width, (float)coordEnd.y / height);

            stringMap[identifier] = {startFloat, endFloat};
            insertionOrder.push_back(identifier);
        }

        
        Sprite& operator[](std::string search){
            if (!stringMap.count(search)){
                std::cout << "[ Key not present in SpriteMap: " << search <<" ]" << std::endl;
                exit(0);
            }
            return stringMap[search];
        }

        Sprite& operator[](int index){
            if (index > Size()){
                std::cout << "[ Index not present in SpriteMap: " << index <<" ]" << std::endl;
                exit(0);
            }
            return stringMap[insertionOrder[index]];
        }

        int Size(){
            return insertionOrder.size();
        }

        void Bind(){
            tex->Bind();
        }

    private:
        Texture* tex{nullptr};

        int width{}, height{};

        std::map<std::string, Sprite> stringMap{};
        std::vector<std::string> insertionOrder{};
};

#endif