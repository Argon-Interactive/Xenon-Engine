#ifndef _XENON_SRC_ECS_SYSTEM_
#define _XENON_SRC_ECS_SYSTEM_

#include "ComponentManager.hpp"
#include "ECS/ComponentID.hpp"
#include "ECS/ComponentsPool.hpp"

namespace Core {
class System {
protected:
	template<typename T>
	LinekdArray<T>& getData(ComponentManager& cmgr, ComponentID ID) { return cmgr.getComponentData(cmgr.getComponent<T>(ID)); }
};
}

#endif
