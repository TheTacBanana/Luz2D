#ifndef GLOBAL_H
#define GLOBAL_H

struct Platform;
struct Renderer;
struct Time;
struct ResourceLoader;
struct InputSystem;

struct GameState;

struct Global{
    Platform* platform;
    InputSystem* inputSystem;
    Renderer* renderer;
    Time* time;
    
    ResourceLoader* resourceLoader;  

    GameState* gameState;
};

extern Global global;

#endif