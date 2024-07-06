#include "scene.hpp"

#include "scenes/scene.hpp"

namespace Xenon {

Scene::Scene(Core::Scene* ptr)
	:m_impl(ptr) {}

void Scene::unload() {
	m_impl->unload();
}

uint64_t Scene::createEntity() {
	return m_impl->createEntity();
}

void Scene::deleteEntity(uint64_t uuid) {
	m_impl->deleteEntity(uuid);
}

void Scene::setBuildIndex(int64_t index) {
	m_impl->setBuildIndex(index);
}

int64_t Scene::getBuildIndex() {
	return m_impl->getBuildIndex();
}

}
