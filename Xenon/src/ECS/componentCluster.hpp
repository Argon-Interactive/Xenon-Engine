#ifndef _XENON_ECS_COMPOENTCLUSTER_
#define _XENON_ECS_COMPOENTCLUSTER_

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

	template<class Component>
	[[nodiscard]] const ComponentPool<Component>& get() const {
		return m_pools.get<Component>();
	}

	[[nodiscard]] ComponentContainerTuple<ComponentPool>& getPools();

private:
	bool m_isLoaded = false;
	std::pmr::memory_resource* m_resource;
	ComponentContainerTuple<ComponentPool> m_pools;

	friend class ComponentManager;
};

}

#endif // !_XENON_ECS_COMPOENTCLUSTER_
