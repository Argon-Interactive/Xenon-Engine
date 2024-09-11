#include "componentCluster.hpp"
#include "core/appData.hpp"

namespace Core {

ComponentCluster::ComponentCluster(std::pmr::memory_resource* resource) : m_resource(resource), m_pools(m_resource) {}

ComponentCluster::~ComponentCluster() { if(m_isLoaded) unload(); }

void ComponentCluster::load() { 
	for_each([](auto& refList, auto& pool){
		refList.push(&pool.m_data);
	}, AppData::getComponentManager().m_refs, m_pools);
	m_isLoaded = true;
}

void ComponentCluster::unload() {
	for_each([](auto& refList, auto& pool){
		refList.pop(&pool.m_data);
	}, AppData::getComponentManager().m_refs, m_pools);
	m_isLoaded = false;
}


ComponentContainerTuple<ComponentPool>& ComponentCluster::getPools() {
	return m_pools;
}

}
