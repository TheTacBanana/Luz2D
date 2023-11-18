#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <cinttypes>
#include <vector>
#include <bitset>
#include <array>
#include "componentbase.hpp"

struct EmptyType{};

int s_componentCounter = 0;

template <class T>
int GetComponentID(){
    static int s_componentID = s_componentCounter++;
    return s_componentID;
}

template<typename T, // For some Type T
         typename I, // With Index Type I
         typename ObjBase, // Obj Base Type
         typename CompBase, // Component Base Type
         int      MaxComponents, // Max Components
         int      MaxEntities // Max Entitiies
         >
struct ComponentManager{
    ComponentManager() = default;

    struct Object : ObjBase{
        public:
            I id = 0;
            std::bitset<MaxComponents> signature{};

            Object() = default;
            Object(T* parent, I id){
                this->p = parent;
                this->id = id;
            }

            inline bool operator==(const Object &rhs) const {
                return this->id == rhs.id;
            }

            inline T& parent() const{
                return *this->p;
            }
            
            template<typename C, class... Args>
            inline C* AddComponent(Args&&... args){
                return p->template AddComponent<C>(id, std::forward<Args>(args)...);
            }

            template<typename C>
            inline C* GetComponent(){
                return p->template GetComponent<C>(id);
            }

            template<typename C>
            inline void RemoveComponent(){
                p->template RemoveComponent<C>(id);
            }

            template<typename C>
            inline C* RequireComponent(){
                if (!HasComponent<C>()){
                    return AddComponent<C>();
                }
                return GetComponent<C>();
            }

            template<typename C>
            inline bool HasComponent(){
                return p->template HasComponent<C>(id);
            }

        private:
            T* p;
    };

    struct Registry {
        std::array<Object, MaxEntities> objects {};
        std::vector<I> freeObjectIDS {};

        Registry(){
            for (I index = 0; index < MaxEntities; index++){
                freeObjectIDS.push_back(index);
            }
        }

        int GetNextIndex(){
            if (!freeObjectIDS.empty()){
                I returnedIndex = freeObjectIDS.back();
                freeObjectIDS.pop_back();
                return returnedIndex;
            } else {
                std::cout << "[ No Free Indexes ]" << std::endl;
                exit(0);
            }
        }
    };

    struct Whirlpool {
        struct ComponentPool {
            ComponentPool(std::size_t elementSize){
                this->elementSize = elementSize;
                pData = new char[elementSize * MaxEntities];
            }

            ~ComponentPool(){
                delete[] pData;
            }

            inline void* get(I index){
                return pData + index * this->elementSize;
            }

            char* pData {nullptr};
            std::size_t elementSize{ 0 };
        };

        std::array<ComponentPool*, MaxComponents> componentPools {nullptr};
        
        template<typename C>
        int GetComponentPoolIndex(){
            int compID = GetComponentID<C>();
            if (compID < MaxComponents){
                if (componentPools[compID] == nullptr){
                    std::cout << "[ New Component Registered ]" << std::endl;
                    componentPools[compID] = new ComponentPool(sizeof(C));
                }
                return compID;
            } else {
                std::cout << "[ Max Components Exceeded ]" << std::endl;
                exit(0);
            }
        }

        template<typename C>
        C* GetComponentAddress(int compID, I index){
            return static_cast<C*>(componentPools[compID]->get(index));
        }
    };

    Registry objectRegistry = Registry();
    Whirlpool componentWhirlpool = Whirlpool();
    
    Object* NewObject(){
        I index = objectRegistry.GetNextIndex();
        objectRegistry.objects[index] = Object(static_cast<T*>(this), index);
        std::cout << "[ Object Created ]" << std::endl;
        return &objectRegistry.objects[index];
    }

    Object* GetObject(I index){
        return &objectRegistry.objects[index];
    }

    void DeleteObject(I index){
        Object& obj = objectRegistry.objects[index];
        obj.signature.reset();
        objectRegistry.freeObjectIDS.push_back(index);
        std::cout << "[ Object Deleted ]" << std::endl;
    }


    template<typename C, class... Args>
    C* AddComponent(I id, Args&&... args){
        int compID = componentWhirlpool.GetComponentPoolIndex<C>();
        Object& obj = objectRegistry.objects[id];

        if (!HasComponent<C>(obj.id)){
            C* component = new (componentWhirlpool.GetComponentAddress<C>(compID, id)) C(std::forward<Args>(args)...);
            obj.signature.set(compID);
            SetupComponent(component, id);
            std::cout << "[ Component Added ]" << std::endl;
            return component;
        } 
        return GetComponent<C>(id);
    }

    template<typename C>
    C* GetComponent(I id){
        int compID = componentWhirlpool.GetComponentPoolIndex<C>();
        Object& obj = objectRegistry.objects[id];

        if (HasComponent<C>(obj.id)){
            return componentWhirlpool.GetComponentAddress<C>(compID, id);
        }
        return nullptr;
    }

    template<typename C>
    void RemoveComponent(I id){
        int compID = componentWhirlpool.GetComponentPoolIndex<C>();
        Object& obj = objectRegistry.objects[id];
        obj.signature.reset(compID);
        std::cout << "[ Component Removed ]" << std::endl;
    }

    template<typename C>
    bool HasComponent(I id){
        int compID = componentWhirlpool.GetComponentPoolIndex<C>();
        Object& obj = objectRegistry.objects[id];
        return obj.signature[compID];
    }

    virtual void SetupComponent(CompBase* comp, I index) = 0;
};

#endif