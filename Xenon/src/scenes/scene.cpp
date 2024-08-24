#include "scene.hpp"
#include "devTools/logger_core.hpp"

#include <thread>

namespace Core {

Scene::Scene()
	: m_sceneMemory("Unnamed Scene"),
	  m_components(m_sceneMemory.get()),
	  m_runtimeCreated(true), m_buildIndex(0) {};

Scene::Scene(uint64_t buildIndex) 
	: m_sceneMemory("Scene " + std::to_string(buildIndex)),
	  m_components(m_sceneMemory.get()),
	  m_runtimeCreated(false), m_buildIndex(buildIndex) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	XN_LOG_TRC("Scene {0}: Loaded", p_debugIndex());
};

Scene::~Scene() {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	XN_LOG_TRC("Scene {0}: Scene destroyed", p_debugIndex());
}

void Scene::unload() {
	XN_LOG_TRC("Scene {0}: Unloading scene", p_debugIndex());
}

Entity Scene::createEntity() {
	return s_entityID--;
}

void Scene::deleteEntity(uint64_t uuid) {
	XN_LOG_TRC("Scene {0}: Deleting entity {0}", p_debugIndex(), uuid);
}

uint64_t Scene::getBuildIndex() const {
	return m_buildIndex;
}



#ifdef __DEBUG__
Scene::SceneMemory::SceneMemory(const std::string& name)
	: mem2("[Upstream] " + name),
	  mem1(&mem2),
	  mem0(name, &mem1) {}
#else 
Scene::SceneMemory::SceneMemory(const std::string& name)
	: mem0({0, 4096}, std::pmr::get_default_resource()) {}
#endif //!__DEBUG__

std::pmr::memory_resource* Scene::SceneMemory::get() {
	return &mem0;
}



}
