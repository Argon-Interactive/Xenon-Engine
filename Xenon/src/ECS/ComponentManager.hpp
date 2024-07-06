#ifndef _XENON_ECS_COMPONENT_MANAGER_
#define _XENON_ECS_COMPONENT_MANAGER_

#include "ComponentID.hpp"
#include "ComponentsPool.hpp"
#include <cstddef>
#include <cstdint>

namespace Core {


class ComponentManager {
public:
	ComponentManager() 
	:m_test(this, ComponentID::Test) {}
	~ComponentManager();
	ComponentManager(const ComponentManager&);
	ComponentManager (ComponentManager&&) noexcept;
	ComponentManager& operator=(const ComponentManager&);
	ComponentManager& operator=(ComponentManager&&) noexcept ;

	void addComponent(ComponentID ID, Entity ent, int data = {});

	void setComponentData(ComponentID ID, Entity ent, int data);

	void removeComponent(ComponentID, Entity ent);

private:
	void resolveRemoval(ComponentID ID, uint64_t oldEntity, size_t newInx);
	


	CompnentPool<int> m_test;
};
}

#endif // !_XENON_ECS_COMPONENT_MANAGER_
