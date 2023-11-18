#ifndef GAMESTATE_H
#define GAMESTATE_H

struct ECS;
struct UIRoot;
struct PhysicsManager;
struct Camera;

struct GameState {
    ECS* ecs;
    UIRoot* uiroot;
    PhysicsManager* physicsManager;

    Camera* gameCamera;
};


#endif