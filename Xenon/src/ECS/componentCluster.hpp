#ifndef _XENON_ECS_COMPOENNTSCLUSTER_
#define _XENON_ECS_COMPOENNTSCLUSTER_

//TEST: temp
#include "componentContainerTuple.hpp"
#include "componentPool.hpp"

namespace Core {

class ComponentCluster {
public:
	explicit ComponentCluster(std::pmr::memory_resource* resource = std::pmr::get_default_resource());
	~ComponentCluster();

	ComponentCluster(ComponentCluster &&) = delete;
	ComponentCluster(const ComponentCluster&) = delete;
	ComponentCluster &operator=(ComponentCluster &&) = delete;
	ComponentCluster &operator=(const ComponentCluster &) = delete;

	void load();
	void unload();

	template<class Component>
	[[nodiscard]] ComponentPool<Component>& get() {
		return m_pools.get<Component>();
	}

private:
	bool m_isLoaded = false;
	std::pmr::memory_resource* m_resource;
	ComponentContainerTuple<ComponentPool> m_pools;

	friend class ComponentManager;
};

}

#endif // !_XENON_ECS_COMPOENNTSCLUSTER_
