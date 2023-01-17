#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <cinttypes>
#include <vector>
#include <bitset>
#include <array>

struct EmptyType{};

int s_componentCounter = 0;

template <class T>
int GetComponentID(){
    static int s_componentID = s_componentCounter++;
    return s_componentID;
}

template<
         typename T, // For some Type T
         typename I, // With Index Type I
         typename ObjBase, // Obj Base Type
         typename CompBase, // Component Base Type
         int      MaxComponents,
         int      MaxEntities
         >
struct ComponentManager{
    ComponentManager() = default;

    // Base Object Type
    struct Object : ObjBase{
        public:
            I id = 0;
            std::bitset<MaxComponents> signature{};

            Object() {}
            Object(T* parent, I id){
                this->p = parent;
                this->id = id;
            }

            inline operator I() const {
                return this->id;
            }

            inline bool operator==(const Object &rhs) const {
                return this->id == rhs.id;
            }

            inline T& parent() const{
                return *this->p;
            }
            
            /*
            template<typename C>
            C* add(C &&component = C()){
                //static_assert(std::is_base_of_v<CompBase, C>);
                return p->AddComponent(this->id, std::forward<C>(component));
            }
            */
            
            template<typename C>
            C* add(){
                return p->template AddComponent<C>(id);
            }

        private:
            T* p;
    };

    // Collection of Registered Objects
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
                std::cout << "No Free Indexes" << std::endl;
                exit(0);
            }
        }
    };

    Registry objectRegistry = Registry();

    Object* NewObject(){
        I index = objectRegistry.GetNextIndex();
        objectRegistry.objects[index] = Object(static_cast<T*>(this), index);
        return &objectRegistry.objects[index];
    }

    void DeleteObject(I index){
        objectRegistry.objects[index].signature.reset();
    }

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

    Whirlpool componentWhirlpool = Whirlpool();
    
    template <typename C>
    C* AddComponent(I id){
        int compID = componentWhirlpool.GetComponentPoolIndex<C>();
        Object& obj = objectRegistry.objects[id];

        if (!obj.signature[compID]){
            C* pComponent = new (componentWhirlpool.GetComponentAddress<C>(compID, id)) C();
            obj.signature.set(compID);
            return pComponent;
        } else {
            std::cout << "[ Component already attatched ]" << std::endl;
            exit(0);
        }
    }
};

#endif