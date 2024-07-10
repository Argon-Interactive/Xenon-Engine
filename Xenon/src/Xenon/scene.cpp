#include "scene.hpp"

#include "scenes/scene.hpp"

namespace Xenon {

Scene::Scene(Core::Scene* ptr)
	:m_impl(ptr) {}

void Scene::unload() {
	m_impl->unload();
}

Entity Scene::createEntity() {
	Entity ent;
	ent.m_uuid = m_impl->createEntity();
	return ent;
}

void Scene::deleteEntity(Entity& entity) {
	m_impl->deleteEntity(entity.m_uuid);
}

int64_t Scene::getBuildIndex() {
	return m_impl->getBuildIndex();
}

}
