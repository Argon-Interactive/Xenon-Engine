#ifndef _XENON_ECS_COMPONENT_MANAGER_
#define _XENON_ECS_COMPONENT_MANAGER_

#include "componentReferenceList.hpp"
#include "ECS/componentImplementations/TestComp.hpp"

#include "System/Transform.hpp"
#include "devTools/logger_core.hpp"

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
		XN_LOG_ERR("Undefined component type");
	}

	ComponentReferenceList<Comp> intCRL;
	ComponentReferenceList<float> floatCRL;
	ComponentReferenceList<Transform> transformCRL;

private:
	ComponentManager() = default;


	friend class AppData;
};

template<>
inline ComponentReferenceList<Comp>& ComponentManager::get<Comp>() {
	return intCRL;
}

template<>
inline ComponentReferenceList<float>& ComponentManager::get<float>() {
	return floatCRL;
}

template<>
inline ComponentReferenceList<Transform>& ComponentManager::get<Transform>() {
	return transformCRL;
}

}

#endif // !_XENON_ECS_COMPONENT_MANAGER_
