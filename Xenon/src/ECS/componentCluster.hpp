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

	void load();
	void unload();

	void syncComponentData() {
		p_performeRemovals(); 
		p_performeAdditions();
		p_resolveDependencies(); 
		p_performeResolvingCleaup();
	}
private:
	bool m_isLoaded = false;

	void p_performeRemovals();
	void p_resolveDependencies();
	void p_performeResolvingCleaup();
	void p_performeAdditions();
};
}
#endif // !_XENON_SRC_ECS_COMPOENNTSCLUSTER_
