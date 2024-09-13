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
	if(hasComponent<Child>(entity))
		detachFromParent(entity);
	while(hasComponent<Parent>(entity)) {
		auto& c = getComponent<Parent>(entity).childList.get();
		deleteEntity(c.m_owner);
	}
	for_each([entity](auto& pool){
		pool.removeComponentOptional(entity);
	}, m_components.getPools());
}

Entity Scene::createChild(Entity parent) {
	auto e = createEntity();
	makeChildOf(parent, e);
	return e;
}

void Scene::makeChildOf(Entity parent, Entity child) {
	if(hasComponent<Child>(child))
		detachFromParent(child);

	addComponent<Child>(child);
	auto& cComp = getComponent<Child>(child);
	cComp.transform.set(getComponent<Transform>(child));
	cComp.parentID = parent;

	if(hasComponent<Parent>(parent)) {
		auto& pComp = getComponent<Parent>(parent);
		auto& first = pComp.childList.get();
		first.prev.set(cComp);
		cComp.next.set(first);
		pComp.childList.set(cComp);
	} else {
		addComponent<Parent>(parent);
		auto& pComp = getComponent<Parent>(parent);
		pComp.childList.set(cComp);
		pComp.transform.set(getComponent<Transform>(parent));
		pComp.root = !hasComponent<Child>(parent);
		if(hasComponent<Child>(parent)) {
			auto& pcComp = getComponent<Child>(parent);
			pcComp.selfParent.set(pComp);
		}
	}
}

void Scene::detachFromParent(Entity child) {
	auto& cComp = getComponent<Child>(child);

	if(cComp.prev.isNull()) {
		if(cComp.next.isNull()) {
			removeComponent<Parent>(cComp.parentID);
		} else {
			cComp.next->prev.set();
			getComponent<Parent>(cComp.parentID).childList.set(cComp.next.get());
		}
	} else {
		cComp.prev->next.set(cComp.next.getPtr());
		if(!cComp.next.isNull())
			cComp.next->prev.set(cComp.prev.get());
	}

	if(hasComponent<Parent>(child)) {
		auto& p = getComponent<Parent>(child);
		p.root = true;
	}
	removeComponent<Child>(child);
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
