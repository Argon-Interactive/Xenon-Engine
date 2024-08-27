#ifndef _XENON_ECS_COMPOENNTSCLUSTER_
#define _XENON_ECS_COMPOENNTSCLUSTER_

//TEST: temp
#include "componentPool.hpp"
#include "ECS/componentImplementations/TestComp.hpp"


namespace Core {

struct ComponentCluster {
	explicit ComponentCluster(std::pmr::memory_resource* resource) : m_resource(resource) {}
	~ComponentCluster();

	ComponentCluster(ComponentCluster &&) = delete;
	ComponentCluster(const ComponentCluster&) = delete;
	ComponentCluster &operator=(ComponentCluster &&) = delete;
	ComponentCluster &operator=(const ComponentCluster &) = delete;

	void load();
	void unload();

	void syncComponentData() {
		p_performRemovals(); 
		p_performAdditions();
		p_resolveDependencies(); 
		p_performResolvingCleaup();
	}
private:
	bool m_isLoaded = false;
	std::pmr::memory_resource* m_resource;

	void p_performRemovals();
	void p_resolveDependencies();
	void p_performResolvingCleaup();
	void p_performAdditions();
public:
	ComponentPool<Comp> intComp{m_resource};
	ComponentPool<float> floatComp{m_resource};
};

}

#endif // !_XENON_ECS_COMPOENNTSCLUSTER_
