#include "scene.hpp"
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
	  m_runtimeCreated(false), m_buildIndex(buildIndex) {};

Scene::~Scene() {
	XN_LOG_DEB("Scene: Scene destroyed");
}

void Scene::unload() {
	XN_LOG_DEB("Unloading scene {0}", m_buildIndex);
}

uint64_t Scene::createEntity() {
	return 0;
}

void Scene::deleteEntity(uint64_t uuid) {
	XN_LOG_DEB("Deleting entity {0} from scene {0}", uuid, m_buildIndex);
}

void Scene::setBuildIndex(uint64_t index) {
	m_buildIndex = index;
}

uint64_t Scene::getBuildIndex() const {
	return m_buildIndex;
}


}
