#ifndef _XENON_ECS_COMPONENT_MANAGER_
#define _XENON_ECS_COMPONENT_MANAGER_

#include "ComponentID.hpp"
#include "ComponentsPool.hpp"
#include "systemTest.hpp"
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

	template<typename T>
	void addComponent(ComponentID ID, Entity ent, T data = {}) {
		switch (ID) {
			case ComponentID::Test:
			m_test.addComponent(ent, data); break;
		}
	}

	template<typename T>
	void setComponentData(ComponentID ID, Entity ent, T data) {
		switch (ID) {
			case ComponentID::Test:
			m_test.setData(ent, data); break;

		}
	}

	void removeComponent(ComponentID ID, Entity ent);
	void purgeComponent(ComponentID ID);
	void purgeAll();
	void runSystems();
private:
	void resolveRemoval(ComponentID ID, uint64_t oldEntity, size_t newInx);

	CompnentPool<int> m_test;
	Core::Systems::testMetadata m_testMetadata;

};
}

#endif // !_XENON_ECS_COMPONENT_MANAGER_
