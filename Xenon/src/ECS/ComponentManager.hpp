#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "ComponentID.hpp"
#include "ComponentsPool.hpp"
#include <cstddef>
#include <cstdint>

namespace Core {


class ComponentManager {
public:
	ComponentManager() 
	:m_test(this, ComponentID::Test)
	{}
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

#endif
