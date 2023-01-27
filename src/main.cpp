#include <iostream>
#include "global.hpp"
#include "util/platform.hpp"
#include "gfx/renderer.hpp"
#include "util/time.hpp"
#include "util/random.hpp"
#include "res/resourceloader.hpp"
#include "input/inputsystem.hpp"
#include "gamestate.hpp"
#include "compsys/ecs.hpp"
#include "physics/physicsmanager.hpp"
#include "util/camera.hpp"
#include "types/transform.hpp"
#include "res/shader.hpp"
#include "res/texture.hpp"
#include "lib/glm/gtx/string_cast.hpp"
#include <vector>
#include <map>

#include "components/ectransform.hpp"
#include "components/ecrigidbody.hpp"
#include "components/ecspriterenderer.hpp"
#include "components/ecaabbcollider.hpp"

Global global;

int main(){
    // Global State Setup ====================================

        Platform platform;
        global.platform = &platform;

        InputSystem inputSystem;
        global.inputSystem = &inputSystem;

            // Renderer ========================

            SpriteRenderer spriteRenderer;
            Renderer renderer {&spriteRenderer};
            global.renderer = &renderer;

            //==================================

        Time time;
        global.time = &time;

        Random random;
        global.random = &random;

        ResourceLoader resourceLoader;
        global.resourceLoader = &resourceLoader;

            // Game State ======================

            GameState gameState;
            global.gameState = &gameState;

            ECS ecs;
            PhysicsManager physicsManager;
            Camera camera;
            gameState.ecs = &ecs;
            gameState.physicsManager = &physicsManager;
            gameState.gameCamera = &camera;

            //==================================

    // Global State Init =====================================

        // Init
        global.resourceLoader->SetResourcePath("Resources/");
        global.renderer->spriteRenderer->Init();

        // Set Camera Position
        global.gameState->gameCamera->transform.position = glm::vec3(0,0,10);
        global.gameState->gameCamera->cameraHeight = 20.0f;

    //========================================================
    
    global.resourceLoader->LoadResource<Texture>("tex", "Textures/crate.png"); 
    Texture* tex = global.resourceLoader->GetResource<Texture>("tex");

    auto floor = global.gameState->ecs->NewObject();
    auto trans = floor->AddComponent<ECTransform>(glm::vec3(0, -5, 0));
    trans->transform.scale = glm::vec3(10, 1, 1);
    floor->AddComponent<ECRigidbody>(false);
    floor->AddComponent<ECBoxCollider>(5, 0.5);
    floor->AddComponent<ECSpriteRenderer>(tex);

    /*
    for (size_t i = 0; i < 20; i++){
        auto obj = global.gameState->ecs->NewObject();
        obj->AddComponent<ECTransform>(glm::vec3(global.random->GetFloat(-5, 5), global.random->GetFloat(-5, 5), 0));
        obj->AddComponent<ECRigidbody>();
        obj->AddComponent<ECBoxCollider>();
        obj->AddComponent<ECSpriteRenderer>(tex);
    }
    */
    
    while (!global.platform->ShouldClose()){
        global.platform->StartFrame();
        
        if (global.inputSystem->GetKeyDown(GLFW_KEY_ESCAPE)) global.platform->Terminate();
        if (global.inputSystem->GetKeyDown(GLFW_KEY_F1)) global.platform->SetDisplayMode(Fullscreen);
        if (global.inputSystem->GetKeyDown(GLFW_KEY_F2)) global.platform->SetDisplayMode(Windowed);     

        float camSpeed = 20.0 * global.time->deltaTime;
        Transform& trans = global.gameState->gameCamera->transform;
        if (global.inputSystem->GetKey('A')) trans.position -= trans.Right() * camSpeed;
        if (global.inputSystem->GetKey('D')) trans.position += trans.Right() * camSpeed;
        if (global.inputSystem->GetKey('W')) trans.position += glm::vec3(0,1,0) * camSpeed;
        if (global.inputSystem->GetKey('S')) trans.position -= glm::vec3(0,1,0) * camSpeed;

        if (global.inputSystem->GetMouseButtonDown(0)){
            auto worldpos = global.gameState->gameCamera->ScreenPositionToWorldSpace(global.inputSystem->mouseAxis);
            auto obj = global.gameState->ecs->NewObject();
            obj->AddComponent<ECTransform>(worldpos);
            obj->AddComponent<ECRigidbody>();
            obj->AddComponent<ECBoxCollider>();
            obj->AddComponent<ECSpriteRenderer>(tex);
        }

        global.gameState->ecs->Update();
        global.gameState->physicsManager->PhysicsUpdate();
        global.gameState->ecs->Render();

        global.platform->EndFrame();
    }
    global.platform->Cleanup();

    return 0;
}