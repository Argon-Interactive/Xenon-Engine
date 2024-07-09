#ifndef _XENON_SRC_ECS_SYSTEM_
#define _XENON_SRC_ECS_SYSTEM_

#include "ComponentManager.hpp"
#include "ECS/ComponentID.hpp"
#include "ECS/ComponentsPool.hpp"

namespace Core {
class System {
protected:
	template<typename T>
	CompnentPool<T>* getComponentPool(ComponentManager& cmgr, ComponentID ID) { return cmgr.getComponent<T>(ID); }
	template<typename T>
	LinekdArray<T>* getData(CompnentPool<T>* componentPool) { return &componentPool->m_data; }
};
}




#endif
