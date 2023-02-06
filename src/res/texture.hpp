#ifndef TEXTURE_H
#define TEXTURE_H

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.hpp"
#endif

#include <glad/glad.h>

#include <string>
#include <iostream>

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

#endif