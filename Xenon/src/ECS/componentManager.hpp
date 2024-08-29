#ifndef _XENON_ECS_COMPONENT_MANAGER_
#define _XENON_ECS_COMPONENT_MANAGER_

#include "componentReferenceList.hpp"
#include "ECS/componentImplementations/TestComp.hpp"

#include "System/Transform.hpp"

namespace Core {

class ComponentManager {
public:
	~ComponentManager() = default;
	ComponentManager(const ComponentManager&);
	ComponentManager (ComponentManager&&) noexcept;
	ComponentManager& operator=(const ComponentManager&);
	ComponentManager& operator=(ComponentManager&&) noexcept ;

	ComponentReferenceList<Comp> intCRL;
	ComponentReferenceList<float> floatCRL;

	ComponentReferenceList<Transform> transformCRL;

private:
	ComponentManager() = default;

	friend class AppData;
};
}

#endif // !_XENON_ECS_COMPONENT_MANAGER_
