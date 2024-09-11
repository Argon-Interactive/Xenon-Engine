#include "scene.hpp"
#include "ECS/containerTuple.hpp"
#include "System/Components/Parenting.hpp"
#include "devTools/logger_core.hpp"

namespace Core {

Scene::Scene()
	: m_sceneMemory("Unnamed Scene"),
	  m_components(m_sceneMemory.get()),
	  m_runtimeCreated(true), m_buildIndex(0) {};

Scene::Scene(uint64_t buildIndex) 
	: m_sceneMemory("Scene " + std::to_string(buildIndex)),
	  m_components(m_sceneMemory.get()),
	  m_runtimeCreated(false), m_buildIndex(buildIndex) {
	m_components.load();
	XN_LOG_TRC("Scene {0}: Loaded", p_debugIndex());

	auto e = createEntity();
	auto& eTransform = getComponent<Transform>(e);
	eTransform.x = 7;
	XN_LOG_DEB("entity position: {0}, {0}", eTransform.x, eTransform.y);
};

Scene::~Scene() {
	m_components.unload();
	XN_LOG_TRC("Scene {0}: Scene destroyed", p_debugIndex());
}

Entity Scene::createEntity() {
	XN_LOG_TRC("Scene {0}: creating entity {0}", p_debugIndex(), static_cast<Entity>(s_entityID));
	if(s_entityID == 0) [[unlikely]]
		s_entityID = s_entityStartID;
	addComponent<Transform>(s_entityID);
	return s_entityID--;
}

void Scene::deleteEntity(Entity entity) {
	XN_LOG_TRC("Scene {0}: Deleting entity {0}", p_debugIndex(), entity);
	for_each([entity](auto& pool){
		pool.removeComponentOptional(entity);
	}, m_components.getPools());
}

uint64_t Scene::getBuildIndex() const {
	return m_buildIndex;
}

void Scene::setEntityStartID(Entity id) {
	s_entityStartID = id++;
}



#ifdef __DEBUG__
Scene::SceneMemory::SceneMemory(const std::string& name)
	: mem2("[Upstream] " + name),
	  mem1(&mem2),
	  mem0(name, &mem1) {}
#else 
Scene::SceneMemory::SceneMemory(const std::string& name)
	: mem0(std::pmr::get_default_resource()) {}
#endif //!__DEBUG__

std::pmr::memory_resource* Scene::SceneMemory::get() {
	return &mem0;
}



}
