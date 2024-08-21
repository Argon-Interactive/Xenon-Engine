#include "scene.hpp"
#include "core/appData.hpp"
#include "devTools/logger_core.hpp"
#include "memory/debugMemoryResource.hpp"

namespace Core {

void Scene::SceneMemory::addMemoryResource(std::unique_ptr<std::pmr::memory_resource>& res) {
	m_mres = res.get();
	m_resourceChain.emplace_back(std::move(res));
}

Scene::SceneMemory::SceneMemory() : m_mres(std::pmr::get_default_resource()) {}

std::pmr::memory_resource* Scene::SceneMemory::get() const noexcept {
	return m_mres;
}

Scene::DynamicSceneMemory::DynamicSceneMemory() {
	const std::pmr::pool_options options = {};
	std::unique_ptr<std::pmr::memory_resource> pool1 = std::make_unique<std::pmr::unsynchronized_pool_resource>(options, m_mres);
	addMemoryResource(pool1);
	std::unique_ptr<std::pmr::memory_resource> debugPool = std::make_unique<DebugMemoryResource>(m_mres);
	addMemoryResource(debugPool);
}

Scene::StaticSceneMemory::StaticSceneMemory() {
	std::unique_ptr<std::pmr::memory_resource> debugPool = std::make_unique<DebugMemoryResource>(m_mres);
	addMemoryResource(debugPool);
}

Scene::Scene() 
	: m_staticComponentChunk(m_staticSceneMemory.get()),
	  m_dynamicComponentChunk(m_dynamicSceneMemory.get()),
	  m_runtimeCreated(true), m_buildIndex(0) {};

Scene::Scene(uint64_t buildIndex) 
	: m_staticComponentChunk(m_staticSceneMemory.get()),
	  m_dynamicComponentChunk(m_dynamicSceneMemory.get()),
	  m_runtimeCreated(false), m_buildIndex(buildIndex) {
	if(buildIndex != 0)
		XN_LOG_WAR("Scene: the scene build number is {0}", buildIndex);

	XN_LOG_DEB("Scene: Perform component cluster tests:");

	m_staticComponentChunk.intComp.emplaceComponent(0, 1, m_staticComponentChunk.floatComp.emplaceComponent(0, 1.5f));
	m_staticComponentChunk.intComp.emplaceComponent(1, 2, m_staticComponentChunk.floatComp.emplaceComponent(1, 2.5f));
	m_staticComponentChunk.intComp.emplaceComponent(2, 3, m_staticComponentChunk.floatComp.emplaceComponent(2, 3.5f));
	m_dynamicComponentChunk.intComp.emplaceComponent(3, 4, m_dynamicComponentChunk.floatComp.emplaceComponent(3, 4.5f));
	m_staticComponentChunk.intComp.emplaceComponent(4, 5, m_staticComponentChunk.floatComp.emplaceComponent(4, 5.5f));
	m_staticComponentChunk.intComp.emplaceComponent(5, 6, m_staticComponentChunk.floatComp.emplaceComponent(5, 6.5f));
	m_dynamicComponentChunk.intComp.emplaceComponent(6, 7, m_dynamicComponentChunk.floatComp.emplaceComponent(6, 7.5f));
	m_dynamicComponentChunk.intComp.emplaceComponent(7, 8, m_dynamicComponentChunk.floatComp.emplaceComponent(7, 8.5f));
	m_dynamicComponentChunk.intComp.emplaceComponent(8, 9, m_dynamicComponentChunk.floatComp.emplaceComponent(8, 9.5f));
	m_dynamicComponentChunk.intComp.emplaceComponent(9, 10, m_dynamicComponentChunk.floatComp.emplaceComponent(9, 10.5f));
	m_staticComponentChunk.intComp.emplaceComponent(10, 11, m_staticComponentChunk.floatComp.emplaceComponent(10, 11.5f));
	m_staticComponentChunk.intComp.emplaceComponent(11, 12, m_staticComponentChunk.floatComp.emplaceComponent(11, 12.5f));
	m_staticComponentChunk.intComp.emplaceComponent(12, 13, m_staticComponentChunk.floatComp.emplaceComponent(12, 13.5f));
	m_staticComponentChunk.intComp.emplaceComponent(13, 14, m_staticComponentChunk.floatComp.emplaceComponent(13, 14.5f));
	m_dynamicComponentChunk.intComp.emplaceComponent(14, 15, m_dynamicComponentChunk.floatComp.emplaceComponent(14, 15.5f));
	m_dynamicComponentChunk.intComp.emplaceComponent(15, 16, m_dynamicComponentChunk.floatComp.emplaceComponent(15, 16.5f));
	m_dynamicComponentChunk.intComp.emplaceComponent(16, 17, m_dynamicComponentChunk.floatComp.emplaceComponent(16, 17.5f));
	m_dynamicComponentChunk.intComp.emplaceComponent(17, 18, m_dynamicComponentChunk.floatComp.emplaceComponent(17, 18.5f));

	m_staticComponentChunk.load();
	m_dynamicComponentChunk.load();

	XN_LOG_ENT("1, 2, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	XN_LOG_DEB("print succesfull");
	m_staticComponentChunk.intComp.removeComponent(1);
	m_staticComponentChunk.floatComp.removeComponent(1);
	m_staticComponentChunk.intComp.removeComponent(5);
	m_staticComponentChunk.floatComp.removeComponent(5);
	m_dynamicComponentChunk.intComp.removeComponent(14);
	m_dynamicComponentChunk.floatComp.removeComponent(14);
	m_staticComponentChunk.floatComp.addComponent(19, 20.5f);
	m_staticComponentChunk.intComp.addComponent(19, {20, nullptr});
	m_staticComponentChunk.syncComponentData();
	m_dynamicComponentChunk.syncComponentData();
	XN_LOG_ENT("1, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	XN_LOG_DEB("print succesfull");
	m_dynamicComponentChunk.floatComp.addComponent(5, 6.5f);
	m_dynamicComponentChunk.intComp.addComponent(5, {6, nullptr});
	m_staticComponentChunk.syncComponentData();
	m_dynamicComponentChunk.syncComponentData();
	XN_LOG_ENT("1, 3, 4");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	XN_LOG_DEB("print succesfull");
	XN_LOG_DEB("system test");
	for(auto &a : AppData::getComponentManager().floatCRL) {
		a += 1.0f;
	}
	m_dynamicComponentChunk.unload();
	XN_LOG_ENT("1, 3");
	for(auto a : AppData::getComponentManager().intCRL) {
		XN_LOG_INF("int: {0}, float: {0}", a.val, *a.ref);
	}
	m_staticComponentChunk.unload();

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
