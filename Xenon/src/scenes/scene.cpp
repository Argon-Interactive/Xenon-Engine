#include "scene.hpp"
#include "devTools/logger_core.hpp"

namespace Core {

Scene::Scene() :m_runtimeCreated(true), m_buildIndex(0) {};

Scene::Scene(uint64_t buildIndex) :m_runtimeCreated(false), m_buildIndex(buildIndex) {};

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
