#ifndef UIROOT_H
#define UIROOT_H

#include <vector>
#include "../compsys/componentmanager.hpp"
#include "../compsys/componentbase.hpp"

using UIElementID = std::uint32_t;
struct UIRoot : ComponentManager<UIRoot,
                                 UIElementID,
                                 EmptyType,
                                 ComponentBase<UIRoot, UIElementID>,
                                 128,
                                 512
                                 > {
    UIRoot() = default;

    void SetupComponent(ComponentBase<UIRoot, UIElementID>* comp, UIElementID index){
        comp->base = this;
        comp->index = index;
        comp->Init();
    }

    void Render(){

    }
};

struct UIComponent : ComponentBase<UIRoot, UIElementID> { };


#endif