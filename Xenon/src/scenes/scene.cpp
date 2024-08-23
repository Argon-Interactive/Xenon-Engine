#include "scene.hpp"
#include "core/appData.hpp"
#include "devTools/logger_core.hpp"

namespace Core {

Scene::SceneMemory::SceneMemory(const std::string& name)
	: debug0("[Upstream] " + name),
	  pool({0, 4096}, &debug0),
	  debug1(name, &pool) {}

std::pmr::memory_resource* Scene::SceneMemory::get() {
	return &debug1;
}

Scene::Scene()
	: m_sceneMemory("Unnamed Scene"),
	  m_components(m_sceneMemory.get()),
	  m_runtimeCreated(true), m_buildIndex(0) {};

Scene::Scene(uint64_t buildIndex) 
	: m_sceneMemory("Scene " + std::to_string(buildIndex)),
	  m_components(m_sceneMemory.get()),
	  m_runtimeCreated(false), m_buildIndex(buildIndex) {
	if(buildIndex != 0)
		XN_LOG_WAR("Scene: the scene build number is {0}", buildIndex);

	XN_LOG_DEB("Scene: Perform component cluster tests:");

	m_components.intComp.emplaceComponent(0, 1, m_components.floatComp.emplaceComponent(0, 1.5f));
	m_components.intComp.emplaceComponent(1, 2, m_components.floatComp.emplaceComponent(1, 2.5f));
	m_components.intComp.emplaceComponent(2, 3, m_components.floatComp.emplaceComponent(2, 3.5f));
	m_components.intComp.emplaceComponent(3, 4, m_components.floatComp.emplaceComponent(3, 4.5f));
	m_components.intComp.emplaceComponent(4, 5, m_components.floatComp.emplaceComponent(4, 5.5f));
	m_components.intComp.emplaceComponent(5, 6, m_components.floatComp.emplaceComponent(5, 6.5f));
	m_components.intComp.emplaceComponent(6, 7, m_components.floatComp.emplaceComponent(6, 7.5f));
	m_components.intComp.emplaceComponent(7, 8, m_components.floatComp.emplaceComponent(7, 8.5f));
	m_components.intComp.emplaceComponent(8, 9, m_components.floatComp.emplaceComponent(8, 9.5f));
	m_components.intComp.emplaceComponent(9, 10, m_components.floatComp.emplaceComponent(9, 10.5f));
	m_components.intComp.emplaceComponent(10, 11, m_components.floatComp.emplaceComponent(10, 11.5f));
	m_components.intComp.emplaceComponent(11, 12, m_components.floatComp.emplaceComponent(11, 12.5f));
	m_components.intComp.emplaceComponent(12, 13, m_components.floatComp.emplaceComponent(12, 13.5f));
	m_components.intComp.emplaceComponent(13, 14, m_components.floatComp.emplaceComponent(13, 14.5f));
	m_components.intComp.emplaceComponent(14, 15, m_components.floatComp.emplaceComponent(14, 15.5f));
	m_components.intComp.emplaceComponent(15, 16, m_components.floatComp.emplaceComponent(15, 16.5f));
	m_components.intComp.emplaceComponent(16, 17, m_components.floatComp.emplaceComponent(16, 17.5f));
	m_components.intComp.emplaceComponent(17, 18, m_components.floatComp.emplaceComponent(17, 18.5f));

	m_components.load();
	m_components.load();

	XN_LOG_ENT("1, 2, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	XN_LOG_DEB("print succesfull");
	m_components.intComp.removeComponent(1);
	m_components.floatComp.removeComponent(1);
	m_components.intComp.removeComponent(5);
	m_components.floatComp.removeComponent(5);
	m_components.intComp.removeComponent(14);
	m_components.floatComp.removeComponent(14);
	m_components.floatComp.addComponent(19, 20.5f);
	m_components.intComp.addComponent(19, {20, nullptr});
	m_components.syncComponentData();
	m_components.syncComponentData();
	XN_LOG_ENT("1, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	XN_LOG_DEB("print succesfull");
	m_components.floatComp.addComponent(5, 6.5f);
	m_components.intComp.addComponent(5, {6, nullptr});
	m_components.syncComponentData();
	m_components.syncComponentData();
	XN_LOG_ENT("1, 3, 4");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	XN_LOG_DEB("print succesfull");
	XN_LOG_DEB("system test");
	for(auto &a : AppData::getComponentManager().floatCRL) {
		a += 1.0f;
	}
	m_components.unload();
	XN_LOG_ENT("1, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	m_components.unload();

	XN_LOG_DEB("Scene: component cluster test completed");
};

Scene::~Scene() {
	XN_LOG_DEB("Scene: Scene destroyed");
}

void Scene::unload() {
	XN_LOG_DEB("Scene: Unloading scene {0}", m_buildIndex);
}

uint64_t Scene::createEntity() {
	return 0;
}

void Scene::deleteEntity(uint64_t uuid) {
	XN_LOG_DEB("Scene: Deleting entity {0} from scene {0}", uuid, m_buildIndex);
}

void Scene::setBuildIndex(uint64_t index) {
	m_buildIndex = index;
}

uint64_t Scene::getBuildIndex() const {
	return m_buildIndex;
}


}
