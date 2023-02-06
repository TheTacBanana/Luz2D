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
#include "components/ectilemap.hpp"

#include "types/spritesheet.hpp"

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
    auto tex = global.resourceLoader->GetResource<Texture>("tex");

    auto floor = global.gameState->ecs->NewObject();
    auto trans = floor->AddComponent<ECTransform>(glm::vec3(0, -5, 0));
    trans->transform.scale = glm::vec3(10, 1, 1);
    floor->AddComponent<ECRigidbody>(false);
    floor->AddComponent<ECBoxCollider>(5, 0.5);
    floor->AddComponent<ECSpriteRenderer>(tex);

    auto player = global.gameState->ecs->NewObject();
    auto playertrans = player->AddComponent<ECTransform>();
    auto playerrb = player->AddComponent<ECRigidbody>(false);
    player->AddComponent<ECBoxCollider>();
    player->AddComponent<ECSpriteRenderer>(tex);

    auto tilemap = global.gameState->ecs->NewObject();
    auto tilemaptrans = tilemap->AddComponent<ECTransform>(glm::vec3(0, 0, -1.0));
    auto tilemapcomp = tilemap->AddComponent<ECTileMap>();

    auto spritesheet = SpriteSheet(tex);
    spritesheet.FixedSlice(8, 8);
    
    while (!global.platform->ShouldClose()){
        global.platform->StartFrame();
        
        if (global.inputSystem->GetKeyDown(GLFW_KEY_ESCAPE)) global.platform->Terminate();
        if (global.inputSystem->GetKeyDown(GLFW_KEY_F1)) global.platform->SetDisplayMode(Fullscreen);
        if (global.inputSystem->GetKeyDown(GLFW_KEY_F2)) global.platform->SetDisplayMode(Windowed);     

        /*
        if (global.inputSystem->GetMouseButtonDown(0)){
            auto worldpos = global.gameState->gameCamera->ScreenPositionToWorldSpace(global.inputSystem->mouseAxis);
            auto obj = global.gameState->ecs->NewObject();
            obj->AddComponent<ECTransform>(worldpos);
            obj->AddComponent<ECRigidbody>();
            obj->AddComponent<ECBoxCollider>();
            obj->AddComponent<ECSpriteRenderer>(tex);
        }
        */

        if (global.inputSystem->GetMouseButtonDown(0)){
            auto worldpos = global.gameState->gameCamera->ScreenPositionToWorldSpace(global.inputSystem->mouseAxis);
            GlobalCoord coord = PosToGlobalCoord(worldpos);
            tilemapcomp->SetTile(coord, true);
        }

        // Lerp to Player Pos
        Transform& trans = global.gameState->gameCamera->transform;
        trans.position = glm::lerp(trans.position, 
                                   playertrans->transform.position + glm::vec3(0,0,10), 
                                   5.0f * time.deltaTime);

        float dx = global.inputSystem->GetKey('A') ? -1.0f : (global.inputSystem->GetKey('D') ? 1.0 : 0);
        float dy = global.inputSystem->GetKey('S') ? -1.0f : (global.inputSystem->GetKey('W') ? 1.0 : 0);
        glm::vec3 movementVector = glm::vec3(dx, dy, 0) * (float)10.0;
        playerrb->velocity = movementVector;

        global.gameState->ecs->Update();
        global.gameState->physicsManager->PhysicsUpdate();
        global.gameState->ecs->Render();

        global.platform->EndFrame();
    }
    global.platform->Cleanup();

    return 0;
}