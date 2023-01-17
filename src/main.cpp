#include "global.hpp"
#include "util/platform.hpp"
#include "gfx/renderer.hpp"
#include "util/time.hpp"
#include "res/resourceloader.hpp"
#include "input/inputsystem.hpp"
#include "gamestate.hpp"
#include "comp/ecs.hpp"
#include "util/camera.hpp"

#include "types/transform.hpp"
#include "res/texture.hpp"

#include <vector>
#include <map>

Global global;
struct ECTransform : EntityComponent<ECTransform>{
    Transform transform;

    ECTransform() = default;
    ECTransform(Transform in){
        this->transform = in;
    }
};

struct ECRigidbody : EntityComponent<ECRigidbody> {
    glm::vec3 velocity;
    glm::vec3 acceleration;

    ECRigidbody() = default;
    ECRigidbody(glm::vec3 velocity, glm::vec3 acceleration){
        this->velocity = velocity;
        this->acceleration = acceleration;
    }
};

struct ECSpriteRenderer : EntityComponent<ECSpriteRenderer> {
    Texture* tex;

    ECSpriteRenderer() = default;
    ECSpriteRenderer(Texture* tex){
        this->tex = tex;
    }
};

int main(){
    {   // Global State Setup
        Platform platform;
        global.platform = &platform;

        InputSystem inputSystem;
        global.inputSystem = &inputSystem;

        Renderer renderer;
        global.renderer = &renderer;

        Time time;
        global.time = &time;

        ResourceLoader resourceLoader;
        global.resourceLoader = &resourceLoader;
        global.resourceLoader->SetResourcePath("Resources/");

        // Game State
        GameState gameState;
        global.gameState = &gameState;

        ECS ecs;
        Camera camera;
        gameState.ecs = &ecs;
        gameState.gameCamera = &camera;
    }

    auto player = global.gameState->ecs->NewObject();
    player->add<ECTransform>();
    player->add<ECRigidbody>();
    player->add<ECSpriteRenderer>();

    auto otherobject = global.gameState->ecs->NewObject();
    otherobject->add<ECTransform>();

    while (!global.platform->ShouldClose()){
        global.platform->StartFrame();

        if (global.inputSystem->GetKey(GLFW_KEY_ESCAPE))
            global.platform->Terminate();
        if (global.inputSystem->GetKeyDown(GLFW_KEY_F1))
            global.platform->SetDisplayMode(Fullscreen);
        if (global.inputSystem->GetKeyDown(GLFW_KEY_F2))
            global.platform->SetDisplayMode(Windowed);      

        global.platform->EndFrame();
    }
    global.platform->Cleanup();

    return 0;
}