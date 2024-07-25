#ifndef _XENON_SRC_ECS_COMPOENNTSCLUSTER_
#define _XENON_SRC_ECS_COMPOENNTSCLUSTER_

#include "ComponentPool.hpp"
#include "ComponentManager.hpp"

namespace Core {
struct ComponentCluster {
	ComponentCluster(std::pmr::memory_resource* memRsrc);
	~ComponentCluster();

	ComponentCluster(ComponentCluster &&) = delete;
	ComponentCluster(const ComponentCluster&) = delete;
	ComponentCluster &operator=(ComponentCluster &&) = delete;
	ComponentCluster &operator=(const ComponentCluster &) = default;

	
private:
	std::pmr::memory_resource* m_resource;
};
}
#endif // !_XENON_SRC_ECS_COMPOENNTSCLUSTER_
