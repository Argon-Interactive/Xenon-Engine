#include "componentCluster.hpp"
#include "core/appData.hpp"

//TEST:

Core::ComponentCluster::ComponentCluster(std::pmr::memory_resource* memres)
: intComp(memres), floatComp(memres) {}

Core::ComponentCluster::~ComponentCluster() { if(m_isLoaded) unload(); }

void Core::ComponentCluster::load() { 
	AppData::getComponentManager().intCRL.push(&intComp.m_data);
	AppData::getComponentManager().floatCRL.push(&floatComp.m_data);
	m_isLoaded = true;
}

void Core::ComponentCluster::unload() {
	AppData::getComponentManager().intCRL.pop(&intComp.m_data);
	AppData::getComponentManager().floatCRL.pop(&floatComp.m_data);
	m_isLoaded = false;
}
 
void Core::ComponentCluster::p_resolveDependencies() {
	intComp.p_resolveDependencies<float>(floatComp.m_movedEnts, [](Comp& comp, float* dep) { comp.ref = dep; });
}

void Core::ComponentCluster::p_performeRemovals() {
	intComp.p_resolveRemovals();
	floatComp.p_resolveRemovals();
}
void Core::ComponentCluster::p_performeResolvingCleaup() {
	intComp.m_movedEnts.clear();
	floatComp.m_movedEnts.clear();
}
void Core::ComponentCluster::p_performeAdditions() {
	intComp.p_resolveAdditions();
	intComp.m_entitiesToAdd.clear();
	floatComp.p_resolveAdditions();
	floatComp.m_entitiesToAdd.clear();
}
