#include "global.hpp"
#include "util/platform.hpp"
#include "gfx/renderer.hpp"
#include "util/time.hpp"
#include "res/resourceloader.hpp"
#include "input/inputsystem.hpp"
#include "gamestate.hpp"
#include "compsys/ecs.hpp"
#include "util/camera.hpp"

#include "types/transform.hpp"
#include "res/shader.hpp"
#include "res/texture.hpp"

#include "lib/glm/gtx/string_cast.hpp"

#include <vector>
#include <map>

Global global;
struct ECTransform : EntityComponent<ECTransform>{
    Transform transform{};

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
    Texture* tex{};

    ECSpriteRenderer(){
        global.gameState->ecs->renderEvent.Subscribe(this, &ECSpriteRenderer::Render);
    }

    ECSpriteRenderer(Texture* tex){
        this->tex = tex;
        global.gameState->ecs->renderEvent.Subscribe(this, &ECSpriteRenderer::Render);
    }

    void Render(){
        global.renderer->spriteRenderer->DrawSprite(tex, *p->GetComponent<Transform>(id)
);
    }
};

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

        ResourceLoader resourceLoader;
        global.resourceLoader = &resourceLoader;

            // Game State ======================
            GameState gameState;
            global.gameState = &gameState;

            ECS ecs;
            Camera camera;
            gameState.ecs = &ecs;
            gameState.gameCamera = &camera;

            //==================================

    // Global State Init =====================================
        global.resourceLoader->SetResourcePath("Resources/");

        global.renderer->spriteRenderer->Init();

        global.gameState->gameCamera->transform.position = glm::vec3(0,0,10);

    //========================================================
    
    global.resourceLoader->LoadResource<Texture>("tex", "Textures/sprite.png"); 
    Texture* tex = global.resourceLoader->GetResource<Texture>("tex");

    auto player = global.gameState->ecs->NewObject();
    player->AddComponent<ECTransform>();
    player->AddComponent<ECRigidbody>();
    auto ecsr = player->AddComponent<ECSpriteRenderer>();
    ecsr->tex = tex;

    while (!global.platform->ShouldClose()){
        global.platform->StartFrame();
        
        if (global.inputSystem->GetKeyDown(GLFW_KEY_ESCAPE)) global.platform->Terminate();
        if (global.inputSystem->GetKeyDown(GLFW_KEY_F1)) global.platform->SetDisplayMode(Fullscreen);
        if (global.inputSystem->GetKeyDown(GLFW_KEY_F2)) global.platform->SetDisplayMode(Windowed);     

        float camSpeed = 10.0 * global.time->deltaTime;
        Transform& trans = global.gameState->gameCamera->transform;
        if (global.inputSystem->GetKey('A')) trans.position -= trans.Right() * camSpeed;
        if (global.inputSystem->GetKey('D')) trans.position += trans.Right() * camSpeed;
        if (global.inputSystem->GetKey('W')) trans.position += glm::vec3(0,1,0) * camSpeed;
        if (global.inputSystem->GetKey('S')) trans.position -= glm::vec3(0,1,0) * camSpeed;

        global.platform->RenderEventQueue();

        global.platform->EndFrame();
    }
    global.platform->Cleanup();

    return 0;
}