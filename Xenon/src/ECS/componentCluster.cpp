#include "componentCluster.hpp"
#include "core/appData.hpp"

//TEST:

Core::ComponentCluster::ComponentCluster(std::pmr::memory_resource* resource) : m_resource(resource), m_pools(m_resource) {}

Core::ComponentCluster::~ComponentCluster() { if(m_isLoaded) unload(); }

void Core::ComponentCluster::load() { 
	for_each([](auto& refList, auto& pool){
		refList.push(&pool.m_data);
	}, AppData::getComponentManager().m_refs, m_pools);
	m_isLoaded = true;
}

void Core::ComponentCluster::unload() {
	for_each([](auto& refList, auto& pool){
		refList.pop(&pool.m_data);
	}, AppData::getComponentManager().m_refs, m_pools);
	m_isLoaded = false;
}

void Core::ComponentCluster::syncComponentData() {
	p_performRemovals(); 
	p_performAdditions();
	// p_resolveDependencies(); 
	p_performResolvingCleaup();
}
 
void Core::ComponentCluster::p_resolveDependencies() {
	// m_pools.get<Comp>().p_resolveDependencies<float>(
	// 	m_pools.get<float>().m_movedEnts,
	// 	[](Comp& comp, float* dep) { comp.ref = dep; });
}

void Core::ComponentCluster::p_performRemovals() {
	for_each([](auto& pool){
		pool.p_resolveRemovals();
	}, m_pools);
}
void Core::ComponentCluster::p_performResolvingCleaup() {
	for_each([](auto& pool){
		pool.m_movedEnts.clear();
	}, m_pools);
}
void Core::ComponentCluster::p_performAdditions() {
	for_each([](auto& pool){
		pool.p_resolveAdditions();
		pool.m_entitiesToAdd.clear();
	}, m_pools);
}
