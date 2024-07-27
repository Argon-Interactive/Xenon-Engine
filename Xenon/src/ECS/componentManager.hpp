#ifndef _XENON_ECS_COMPONENT_MANAGER_
#define _XENON_ECS_COMPONENT_MANAGER_

#include "componentPool.hpp"

namespace Core {

class ComponentManager {
public:
	ComponentManager() = default;
	~ComponentManager() = default;
	ComponentManager(const ComponentManager&);
	ComponentManager (ComponentManager&&) noexcept;
	ComponentManager& operator=(const ComponentManager&);
	ComponentManager& operator=(ComponentManager&&) noexcept ;

private:

};
}

#endif // !_XENON_ECS_COMPONENT_MANAGER_
