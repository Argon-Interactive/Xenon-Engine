#ifndef _XENON_SRC_ECS_COMPOENNTSCLUSTER_
#define _XENON_SRC_ECS_COMPOENNTSCLUSTER_

//TEST: temp
#include "componentPool.hpp"
#include "ECS/componentImplementations/TestComp.hpp"


namespace Core {
struct ComponentCluster {
	explicit ComponentCluster(std::pmr::memory_resource* memres = std::pmr::get_default_resource());
	~ComponentCluster();

	ComponentCluster(ComponentCluster &&) = delete;
	ComponentCluster(const ComponentCluster&) = delete;
	ComponentCluster &operator=(ComponentCluster &&) = delete;
	ComponentCluster &operator=(const ComponentCluster &) = delete;

	ComponentPool<Comp> intComp;
	ComponentPool<float> floatComp;

	void syncComponentData() { p_resolveRemovals(); p_resolveDependencies(); }
private:
	void p_resolveRemovals();
	void p_resolveDependencies();
};
}
#endif // !_XENON_SRC_ECS_COMPOENNTSCLUSTER_
