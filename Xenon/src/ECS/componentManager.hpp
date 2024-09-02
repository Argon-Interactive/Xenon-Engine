#ifndef _XENON_ECS_COMPONENTMANAGER_
#define _XENON_ECS_COMPONENTMANAGER_

#include "componentReferenceList.hpp"
#include "componentContainerTuple.hpp"

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

	ComponentContainerTuple<ComponentReferenceList> m_refs;

	friend class AppData;
	friend class ComponentCluster;
};

}

#endif // !_XENON_ECS_COMPONENTMANAGER_
