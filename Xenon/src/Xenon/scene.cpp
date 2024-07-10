#include "scene.hpp"

#include "scenes/scene.hpp"

namespace Xenon {

Scene::Scene(Core::Scene* ptr)
	:m_impl(ptr) {}

void Scene::unload() {
	m_impl->unload();
}

Entity Scene::createEntity() {
	return Entity(m_impl->createEntity());
}

uint64_t Scene::getBuildIndex() {
	return m_impl->getBuildIndex();
}

}
