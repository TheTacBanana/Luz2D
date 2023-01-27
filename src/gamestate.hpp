#ifndef GAMESTATE_H
#define GAMESTATE_H

struct ECS;
struct PhysicsManager;
struct Camera;

struct GameState {
    ECS* ecs;
    PhysicsManager* physicsManager;

    Camera* gameCamera;
};


#endif