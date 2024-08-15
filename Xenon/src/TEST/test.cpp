#include "test.hpp"
#include "core/appData.hpp"
#include "ECS/componentCluster.hpp"
#include "devTools/logger_core.hpp"

void Core::Test::test() {
	ComponentCluster cl1;
	ComponentCluster cl2;

	
	cl1.intComp.emplaceComponent(0, 1, cl1.floatComp.emplaceComponent(0, 1.5f));
	cl1.intComp.emplaceComponent(1, 2, cl1.floatComp.emplaceComponent(1, 2.5f));
	cl1.intComp.emplaceComponent(2, 3, cl1.floatComp.emplaceComponent(2, 3.5f));
	cl2.intComp.emplaceComponent(3, 4, cl2.floatComp.emplaceComponent(3, 4.5f));
	cl1.intComp.emplaceComponent(4, 5, cl1.floatComp.emplaceComponent(4, 5.5f));
	cl1.intComp.emplaceComponent(5, 6, cl1.floatComp.emplaceComponent(5, 6.5f));
	cl2.intComp.emplaceComponent(6, 7, cl2.floatComp.emplaceComponent(6, 7.5f));
	cl2.intComp.emplaceComponent(7, 8, cl2.floatComp.emplaceComponent(7, 8.5f));
	cl2.intComp.emplaceComponent(8, 9, cl2.floatComp.emplaceComponent(8, 9.5f));
	cl2.intComp.emplaceComponent(9, 10, cl2.floatComp.emplaceComponent(9, 10.5f));
	cl1.intComp.emplaceComponent(10, 11, cl1.floatComp.emplaceComponent(10, 11.5f));
	cl1.intComp.emplaceComponent(11, 12, cl1.floatComp.emplaceComponent(11, 12.5f));
	cl1.intComp.emplaceComponent(12, 13, cl1.floatComp.emplaceComponent(12, 13.5f));
	cl1.intComp.emplaceComponent(13, 14, cl1.floatComp.emplaceComponent(13, 14.5f));
	cl2.intComp.emplaceComponent(14, 15, cl2.floatComp.emplaceComponent(14, 15.5f));
	cl2.intComp.emplaceComponent(15, 16, cl2.floatComp.emplaceComponent(15, 16.5f));
	cl2.intComp.emplaceComponent(16, 17, cl2.floatComp.emplaceComponent(16, 17.5f));
	cl2.intComp.emplaceComponent(17, 18, cl2.floatComp.emplaceComponent(17, 18.5f));

	cl1.load();
	cl2.load();

	XN_LOG_ENT("1, 2, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	XN_LOG_DEB("print succesfull");
	cl1.intComp.removeComponent(1);
	cl1.floatComp.removeComponent(1);
	cl1.intComp.removeComponent(5);
	cl1.floatComp.removeComponent(5);
	cl2.intComp.removeComponent(14);
	cl2.floatComp.removeComponent(14);
	cl1.floatComp.addComponent(19, 20.5f);
	cl1.intComp.addComponent(19, {20, nullptr});
	cl1.syncComponentData();
	cl2.syncComponentData();
	XN_LOG_ENT("1, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	XN_LOG_DEB("print succesfull");
	cl2.floatComp.addComponent(5, 6.5f);
	cl2.intComp.addComponent(5, {6, nullptr});
	cl1.syncComponentData();
	cl2.syncComponentData();
	XN_LOG_ENT("1, 3, 4");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	XN_LOG_DEB("print succesfull");
	XN_LOG_DEB("system test");
	for(auto &a : AppData::getComponentManager().floatCRL) {
		a += 1.0f;
	}
	cl2.unload();
	XN_LOG_ENT("1, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	cl1.unload();
}
