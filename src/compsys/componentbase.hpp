#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

template <typename C, // Base Type 
          typename I // Index Type
          >
struct ComponentBase{
    C* base;
    I index;  

    virtual void Init(){};
};

#endif