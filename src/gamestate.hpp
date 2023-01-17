#ifndef GAMESTATE_H
#define GAMESTATE_H

struct ECS;
struct Camera;

struct GameState {
    ECS* ecs;

    Camera* gameCamera;
};


#endif