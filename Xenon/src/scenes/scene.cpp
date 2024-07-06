#include "scene.hpp"
#include "devTools/logger_core.hpp"

namespace Core {

Scene::Scene() = default;

Scene::~Scene() {
	XN_LOG_DEB("Scene: Scene destroyed");
}

void Scene::unload() {

}

uint64_t Scene::createEntity() {
	return 0;
}

void Scene::deleteEntity([[maybe_unused]]uint64_t uuid) {
	
}

void Scene::setBuildIndex(int64_t index) {
	m_buildIndex = index;
}

int64_t Scene::getBuildIndex() const {
	return m_buildIndex;
}


}
