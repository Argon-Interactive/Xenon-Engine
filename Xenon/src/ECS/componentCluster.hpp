#ifndef _XENON_ECS_COMPOENNTSCLUSTER_
#define _XENON_ECS_COMPOENNTSCLUSTER_

//TEST: temp
#include "componentPool.hpp"
#include "ECS/componentImplementations/TestComp.hpp"
#include "System/Transform.hpp"


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
	void syncComponentData();

	template<class Component>
	[[nodiscard]] ComponentPool<Component>& get() {
		XN_LOG_ERR("Undefined component type");
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
	ComponentPool<Transform> transformComp{m_resource};
};

template<>
inline ComponentPool<Comp>& ComponentCluster::get<Comp>() {
	return intComp;
}

template<>
inline ComponentPool<float>& ComponentCluster::get<float>() {
	return floatComp;
}

template<>
inline ComponentPool<Transform>& ComponentCluster::get<Transform>() {
	return transformComp;
}

}

#endif // !_XENON_ECS_COMPOENNTSCLUSTER_
