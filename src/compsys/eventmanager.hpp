#include <vector>
#include "../global.hpp"

struct HandlerFunctionBase {
  	public:
		void Exec() {
			this->Call();
		}
	
		virtual void Call() = 0;
};

template<class T>
struct MemberFunctionHandler : public HandlerFunctionBase{
  	public:
		typedef void (T::*MemberFunction)();

		MemberFunctionHandler(T * instance, MemberFunction memberFunction){
			this->instance = instance;
			this->memberFunction = memberFunction;
		}

		void Call() {
			(instance->*memberFunction)();
		}
  	private:
    	T* instance;
      	MemberFunction memberFunction;
};

struct EventManager {
  	public:
		std::vector<HandlerFunctionBase*> handlers{};

		void Publish() {
			for (auto& handler : handlers)
				if (handler != nullptr) 
					handler->Call();
		}

		template<class T>
		void Subscribe(T * instance, void (T::*memberFunction)() ) {
			handlers.push_back(new MemberFunctionHandler<T>(instance, memberFunction));
		}
};