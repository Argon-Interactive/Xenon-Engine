#ifndef _XENON_ECS_COMPONENT_MANAGER_
#define _XENON_ECS_COMPONENT_MANAGER_

#include "componentReferenceList.hpp"
#include "componentTuple.hpp"

namespace Core {

class ComponentManager {
public:
	~ComponentManager() = default;
	ComponentManager(const ComponentManager&);
	ComponentManager (ComponentManager&&) noexcept;
	ComponentManager& operator=(const ComponentManager&);
	ComponentManager& operator=(ComponentManager&&) noexcept ;

	template<class Component>
	[[nodiscard]] ComponentReferenceList<Component>& get() {
		m_refs.get<Component>();
	}

private:
	ComponentManager();

	ComponentReferenceListTuple m_refs;

	friend class AppData;
	friend struct ComponentCluster;
};

}

#endif // !_XENON_ECS_COMPONENT_MANAGER_
