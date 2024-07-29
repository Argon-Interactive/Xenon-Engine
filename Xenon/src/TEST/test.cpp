#include "test.hpp"
#include "core/appData.hpp"
#include "ECS/componentCluster.hpp"
#include "devTools/logger_core.hpp"

void Core::Test::test() {
	ComponentCluster cl;

	cl.floatComp.emplaceComponent(0, 1.5f);
	cl.floatComp.emplaceComponent(1, 2.5f);
	cl.floatComp.emplaceComponent(2, 3.5f);
	
	cl.intComp.emplaceComponent(0, 1, cl.floatComp.getComponentPtr(0));
	cl.intComp.emplaceComponent(1, 2, cl.floatComp.getComponentPtr(1));
	cl.intComp.emplaceComponent(2, 3, cl.floatComp.getComponentPtr(2));


	XN_LOG_ENT("1, 2, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	cl.intComp.removeComponent(1);
	cl.floatComp.removeComponent(1);
	cl.syncComponentData();
	XN_LOG_ENT("1, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}


}
