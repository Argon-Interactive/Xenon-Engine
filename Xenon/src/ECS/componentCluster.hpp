#ifndef _XENON_ECS_COMPOENNTSCLUSTER_
#define _XENON_ECS_COMPOENNTSCLUSTER_

//TEST: temp
#include "componentTuple.hpp"
#include "componentPool.hpp"

namespace Core {

struct ComponentCluster {
public:
	explicit ComponentCluster(std::pmr::memory_resource* resource = std::pmr::get_default_resource());
	~ComponentCluster();

	ComponentCluster(ComponentCluster &&) = delete;
	ComponentCluster(const ComponentCluster&) = delete;
	ComponentCluster &operator=(ComponentCluster &&) = delete;
	ComponentCluster &operator=(const ComponentCluster &) = delete;

	void load();
	void unload();
	void syncComponentData();

	template<class Component>
	[[nodiscard]] ComponentPool<Component>& get() {
		return m_pools.get<Component>();
	}

private:
	bool m_isLoaded = false;
	std::pmr::memory_resource* m_resource;
	ComponentPoolTuple m_pools;

	void p_performRemovals();
	void p_resolveDependencies();
	void p_performResolvingCleaup();
	void p_performAdditions();

	friend class ComponentManager;
};

}

#endif // !_XENON_ECS_COMPOENNTSCLUSTER_
