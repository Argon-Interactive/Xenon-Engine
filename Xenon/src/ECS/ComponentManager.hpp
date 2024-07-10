#ifndef _XENON_ECS_COMPONENT_MANAGER_
#define _XENON_ECS_COMPONENT_MANAGER_
#include "ComponentID.hpp"
#include "ComponentPool.hpp"
#include <cstddef>
#include <cstdint>

namespace Core {

class ComponentManager {
public:
	ComponentManager() 
	:m_test(this, ComponentID::Test) {}
	~ComponentManager() = default;
	ComponentManager(const ComponentManager&);
	ComponentManager (ComponentManager&&) noexcept;
	ComponentManager& operator=(const ComponentManager&);
	ComponentManager& operator=(ComponentManager&&) noexcept ;

	template<typename T>
	[[nodiscard]] ComponentPool<T>& getComponent(ComponentID ID) {
		switch (ID) {
			case ComponentID::Test:
			return m_test;
		
		}
	}

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

	void addComponent(ComponentID ID, Entity ent);
	void removeComponent(ComponentID ID, Entity ent);
	void purgeComponent(ComponentID ID);
	void purgeAll();
private:
	void resolveRemoval(ComponentID ID, uint64_t oldEntity, uint32_t newInxMajor, uint32_t newInxMinor);
	template<typename T>
	LinkedArray<T>& getComponentData(ComponentPool<T>& CP) { return CP.m_data; }

	ComponentPool<int> m_test;
	friend class System;
};
}

#endif // !_XENON_ECS_COMPONENT_MANAGER_
