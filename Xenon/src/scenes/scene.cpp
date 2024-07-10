#include "scene.hpp"
#include "devTools/logger_core.hpp"

namespace Core {

Scene::Scene() = default;

Scene::~Scene() {
	XN_LOG_DEB("Scene: Scene destroyed");
}

void Scene::unload() {

}

Entity Scene::createEntity() {
	return 0;
}

void Scene::deleteEntity(Entity uuid) {
	
}

void Scene::setBuildIndex(int64_t index) {
	m_buildIndex = index;
}

int64_t Scene::getBuildIndex() const {
	return m_buildIndex;
}


}
