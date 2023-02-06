#ifndef ECTILEMAP_H
#define ECTILEMAP_H

#include "../compsys/ecs.hpp"
#include "../types/transform.hpp"
#include "../res/texture.hpp"
#include "../global.hpp"
#include "../gamestate.hpp"
#include "../gfx/renderer.hpp"
#include "../lib/glm/glm.hpp"
#include <unordered_map>
#include <map>
#include <utility>
#include <vector>

#include "ectransform.hpp"

struct TileMapVertex{
    glm::vec3 pos;
    glm::vec2 texCoord;
};

using GlobalCoord = std::pair<int, int>; // Global coordinate
using LocalCoord = std::pair<int, int>; // Within Chunk
using ChunkCoord = std::pair<int, int>; // Index of Chunks in the world
using ChunkLocalPair = std::pair<ChunkCoord, LocalCoord>; // Pair of coords

inline GlobalCoord PosToGlobalCoord(glm::vec3 pos){
    return GlobalCoord(std::floor(pos.x), std::floor(pos.y));
}

inline int PosMod(int in, int mod){
    int val = in % mod; 
    if (val < 0) 
        val += mod; 
    return val; 
}

struct ECTileMap : EntityComponent {
    public:
        const static int CHUNK_WIDTH = 16;
        const static int CHUNK_HEIGHT = 16;

        struct TileMapChunk{
            public:
                TileMapChunk(ChunkCoord coord){
                    this->coord = coord;
                }

                void GenerateChunkMesh(){
                    vertices.clear();
                    
                    for (size_t i = 0; i < CHUNK_WIDTH * CHUNK_HEIGHT; i++){
                        auto value = contents[i];
                        if (value){
                            float x = i % CHUNK_WIDTH;
                            float y = floor((float)i / CHUNK_HEIGHT); 

                            // Tri 1
                            vertices.push_back(glm::vec3(x, y, 0.0));
                            vertices.push_back(glm::vec3(x + 1.0, y, 0.0));
                            vertices.push_back(glm::vec3(x, y + 1.0, 0.0));

                            // Tri 2
                            vertices.push_back(glm::vec3(x + 1.0, y, 0.0));
                            vertices.push_back(glm::vec3(x + 1.0, y + 1.0, 0.0));
                            vertices.push_back(glm::vec3(x, y + 1.0, 0.0));
                        }
                    }

                    glGenVertexArrays(1, &VAO);
                    glGenBuffers(1, &VBO);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

                    glBindVertexArray(VAO);
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

                    glBindBuffer(GL_ARRAY_BUFFER, 0); 
                    glBindVertexArray(0); 

                    canRender = true;
                }

                void SetTile(LocalCoord coord, bool value){
                    contents[coord.first + coord.second * CHUNK_WIDTH] = value;
                    GenerateChunkMesh();
                }

                void Render(Shader* shader){
                    if (canRender){
                        glm::mat4 transform = glm::mat4(1.0);
                        transform = glm::translate(transform, glm::vec3(coord.first * 16, coord.second * 16, 0));
                        shader->setMat4("chunktransform", transform);

                        glBindVertexArray(VAO);
                        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
                        glBindVertexArray(0);
                    }
                }

            private:
                std::vector<glm::vec3> vertices{};

                ChunkCoord coord{};
                unsigned int VBO, VAO;
                bool canRender{false};

                std::array<bool, CHUNK_WIDTH * CHUNK_HEIGHT> contents {false};
        };

        ECTileMap(){
            global.gameState->ecs->renderEvent.Subscribe(this, &ECTileMap::Render);

            global.resourceLoader->LoadResource<Shader>("TileMapShader", "Shaders/tilemap.vs", "Shaders/tilemap.fs");
            tileMapShader = global.resourceLoader->GetResource<Shader>("TileMapShader");
        }

        void SetTile(GlobalCoord coord, bool value){
            ChunkLocalPair pair = GetChunkLocalPair(coord);

            TileMapChunk& chunk = GetChunk(pair.first);
            chunk.SetTile(pair.second, value);
        }

        void Render(){
            tileMapShader->use();

            auto transform = base->GetComponent<ECTransform>(index)->transform;
            tileMapShader->setMat4("model", transform.GetModelMatrix());
            tileMapShader->setMat4("view", global.gameState->gameCamera->GetViewMatrix());
            tileMapShader->setMat4("projection", global.gameState->gameCamera->GetOrthoMatrix());

            for (const auto& pair : chunks) {
                TileMapChunk* chunk = pair.second;
                chunk->Render(tileMapShader);
            }
        }

    private:
        Shader* tileMapShader {nullptr};

        std::map<ChunkCoord, TileMapChunk*> chunks;

        TileMapChunk& GetChunk(ChunkCoord coord){
            if (chunks.count(coord)){
                return *chunks[coord];
            }
            chunks[coord] = new TileMapChunk(coord);
            return *chunks[coord];
        }

        ChunkLocalPair GetChunkLocalPair(GlobalCoord coord){
            LocalCoord local (PosMod(coord.first, CHUNK_WIDTH), 
                              PosMod(coord.second, CHUNK_HEIGHT)); 
            ChunkCoord chunk (floor((float)coord.first / CHUNK_WIDTH), 
                              floor((float)coord.second / CHUNK_HEIGHT));
            return ChunkLocalPair(chunk, local);
        }
};

#endif