#ifndef ECSPRITERENDERER_H
#define ECSPRITERENDERER_H

#include "../compsys/ecs.hpp"
#include "../types/transform.hpp"
#include "../res/texture.hpp"
#include "../global.hpp"
#include "../gamestate.hpp"
#include "../gfx/renderer.hpp"

struct ECSpriteRenderer : EntityComponent {
    Texture* tex;

    ECSpriteRenderer(Texture* texIn){
        tex = texIn;
        global.gameState->ecs->renderEvent.Subscribe(this, &ECSpriteRenderer::Render);
    }

    void Render(){
        auto transform = base->GetComponent<ECTransform>(index)->transform;
        global.renderer->spriteRenderer->DrawSprite(tex, transform);
    }
};

#endif