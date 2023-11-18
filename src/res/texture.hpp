#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.hpp"
#endif

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <map>
#include <string>

struct Sprite{
    unsigned int texID;

    glm::vec2 coordStart;
    glm::vec2 coordEnd;

    glm::ivec2 size;

    glm::vec2 origin {glm::vec2(0.5, 0.5)};

    void Bind(){
        glBindTexture(GL_TEXTURE_2D, texID);
    }
};

struct Texture{
    public:
        int width{}, height{};

        Texture(std::string path){
            LoadTexture(path);

            CreateSlice("default", glm::vec2{0.0, 0.0}, glm::ivec2(width, height));
        }

        void Bind(){
            glBindTexture(GL_TEXTURE_2D, texID);
        }

        void CreateSlice(std::string identifier, glm::ivec2 coordStart, glm::ivec2 size){
            glm::ivec2 coordEnd = coordStart + size;

            glm::vec2 startFloat = glm::vec2((float)coordStart.x / width, (float)coordStart.y / height);
            glm::vec2 endFloat = glm::vec2((float)coordEnd.x / width, (float)coordEnd.y / height);

            stringMap[identifier] = {texID, startFloat, endFloat, size};
            insertionOrder.push_back(identifier);
        }

        Sprite& operator[](std::string search){
            if (!stringMap.count(search)){
                std::cout << "[ Key not present in SpriteMap: " << search <<" ]" << std::endl;
                exit(0);
            }
            return stringMap[search];
        }

        int SpriteCount(){
            return insertionOrder.size();
        }

    private:
        unsigned int texID{};

        std::map<std::string, Sprite> stringMap{};
        std::vector<std::string> insertionOrder{};

        void LoadTexture(std::string texPath){
            glGenTextures(1, &texID);

            glBindTexture(GL_TEXTURE_2D, texID);

            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            // load and generate the texture
            int nrChannels;
            unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);

            if (data){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            } else {
                std::cout << "Failed to load texture at: " << texPath << std::endl;
                exit(0);
            }
            stbi_image_free(data);
        }

};

/*
struct Texture{
    public:
        Texture() = default;

        Texture(std::string texPath){
            LoadTexture(texPath);
        }

        void LoadTexture(std::string texPath){
            glGenTextures(1, &texID);

            glBindTexture(GL_TEXTURE_2D, texID);

            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            // load and generate the texture
            int nrChannels;
            unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);

            if (data){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            } else {
                std::cout << "Failed to load texture at: " << texPath << std::endl;
                exit(0);
            }
            stbi_image_free(data);
        }

        unsigned int GetID(){
            return texID;
        }

        void Bind(){
            glBindTexture(GL_TEXTURE_2D, texID);
        }

        int Width() const{
            return width;
        }

        int Height() const{
            return height;
        }

    private:
        unsigned int texID{};

        int width{}, height{};
};
*/

#endif