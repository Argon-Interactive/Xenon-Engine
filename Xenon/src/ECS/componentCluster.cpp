#include "componentCluster.hpp"
#include "core/appData.hpp"

//TEST:

Core::ComponentCluster::ComponentCluster(std::pmr::memory_resource* memres) : 
intComp(memres), floatComp(memres) 
{
	AppData::getComponentManager().intCRL.push(&intComp.m_data);
	AppData::getComponentManager().floatCRL.push(&floatComp.m_data);
}

Core::ComponentCluster::~ComponentCluster() {
	AppData::getComponentManager().intCRL.pop(&intComp.m_data);
	AppData::getComponentManager().floatCRL.pop(&floatComp.m_data);
}

void Core::ComponentCluster::p_resolveRemovals() {
	intComp.p_resolveRemovals();
	floatComp.p_resolveRemovals();
}
 
void Core::ComponentCluster::p_resolveDependencies() {
	//TODO: Implement a better way of writeing this, I was thinkink about something with lambdas
	for(auto pair : floatComp.m_movedEnts) {
		intComp.getComponent(pair.first).ref = pair.second;
	}
	intComp.m_movedEnts.clear();
	floatComp.m_movedEnts.clear();
}
