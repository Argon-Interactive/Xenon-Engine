#include "sceneManager.hpp"
#include "devTools/logger_core.hpp"
#include "Xenon/scene.hpp"

#include <cstdint>

namespace Core {

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

/*------------------------------------------------------------------------------------------------*/
/*                                         Basic Methods                                          */
/*------------------------------------------------------------------------------------------------*/

Scene* SceneManager::createScene() {
	m_scenes.emplace_back(std::make_unique<Scene>());
	return m_scenes.back().get();
}

void SceneManager::purge() {
	while(!m_scenes.empty()) {
		m_scenes.back()->unload();
		m_scenes.pop_back();
	}
}

Scene* SceneManager::getScene(uint64_t index) const {
	return m_scenes[index].get();
}

Scene* SceneManager::getSceneByBuildIndex(uint64_t buildIndex) const {
	for(const auto& scene : m_scenes)
		if(scene->getBuildIndex() == buildIndex)
			return scene.get();
	XN_LOG_ERR("SceneManager: Scene with build index {0} is not currently loaded.", buildIndex);
	return nullptr;
}

uint64_t SceneManager::getSceneCount() const {
	 return m_scenes.size();
}

/*------------------------------------------------------------------------------------------------*/
/*                                       Loading/Unloading                                        */
/*------------------------------------------------------------------------------------------------*/

void SceneManager::loadScene(uint64_t buildIndex) {
	XN_LOG_DEB("SceneManager: Loading scene {0}", buildIndex);
	m_scenes.emplace_back(std::make_unique<Scene>(buildIndex));
}

void SceneManager::unloadSceneAt(uint64_t index) {
	XN_LOG_DEB("SceneManager: Unloading scene at {0}", index);
	if(index == m_scenes.size() - 1)
		m_activeSceneIndex--;
	getScene(index)->unload();
	p_deleteScene(index);
}

void SceneManager::unloadScene(Scene* scene) {
	XN_LOG_DEB("SceneManager: Unloading scene {0}", scene);
	const uint64_t index = p_getSceneIndex(scene);
	unloadSceneAt(index);
}

void SceneManager::unloadScene(uint64_t buildIndex) {
	XN_LOG_DEB("SceneManager: Unloading scene {0}", buildIndex);
	const uint64_t index = p_getSceneIndex(buildIndex);
	unloadSceneAt(index);
}

void SceneManager::loadSceneAsync([[maybe_unused]] uint64_t buildIndex) {
	XN_LOG_ERR("SceneManager: Do not use SceneManager::loadSceneAsync. This method does not have any implementation.");
}

void SceneManager::unloadSceneAsyncAt([[maybe_unused]] uint64_t index) {
	XN_LOG_ERR("SceneManager: Do not use SceneManager::unloadSceneAsyncAt. This method does not have any implementation.");
}

void SceneManager::unloadSceneAsync([[maybe_unused]] Scene* scene) {
	XN_LOG_ERR("SceneManager: Do not use SceneManager::unloadSceneAsync. This method does not have any implementation.");
}

void SceneManager::unloadSceneAsync([[maybe_unused]] uint64_t buildIndex) {
	XN_LOG_ERR("SceneManager: Do not use SceneManager::unloadSceneAsync. This method does not have any implementation.");
}

/*------------------------------------------------------------------------------------------------*/
/*                                          Active scene                                          */
/*------------------------------------------------------------------------------------------------*/

void SceneManager::setActiveSceneAt(uint64_t index) {
	m_activeSceneIndex = index;
}

void SceneManager::setActiveScene(Scene* scene) {
	m_activeSceneIndex = p_getSceneIndex(scene);
}

void SceneManager::setActiveScene(uint64_t buildIndex) {
	m_activeSceneIndex = p_getSceneIndex(buildIndex);
}

Scene* SceneManager::getActiveScene() const {
	return m_scenes[m_activeSceneIndex].get();
}

uint64_t SceneManager::getActiveSceneIndex() const {
	return m_activeSceneIndex;
}

/*------------------------------------------------------------------------------------------------*/
/*                                        Moving Entities                                         */
/*------------------------------------------------------------------------------------------------*/

void SceneManager::moveEntity([[maybe_unused]] Entity entity, [[maybe_unused]] uint64_t targetSceneIndex) {
	XN_LOG_ERR("SceneManager: Do not use SceneManager::moveEntity. This method does not have any implementation.");
}

void SceneManager::moveEntity([[maybe_unused]] Entity entity, [[maybe_unused]] Scene* targetScene) {
	XN_LOG_ERR("SceneManager: Do not use SceneManager::moveEntity. This method does not have any implementation.");
}



/*------------------------------------------------------------------------------------------------*/
/*                                         Private methods                                        */
/*------------------------------------------------------------------------------------------------*/


void SceneManager::p_moveLoaded([[maybe_unused]] uint64_t index) {

}

void SceneManager::p_moveUnloaded([[maybe_unused]] uint64_t index) {

}

void SceneManager::p_deleteScene(uint64_t index) {
	m_scenes.erase(m_scenes.begin() + static_cast<int64_t>(index));
}

uint64_t SceneManager::p_getSceneIndex(uint64_t buildIndex) {
	const uint64_t max = m_scenes.size();
	for(uint64_t i = 0; i < max; i++)
		if(getScene(i)->getBuildIndex() == buildIndex)
			return i;
	XN_LOG_ERR("SceneManager: Scene of build index {0} is not loaded.", buildIndex);
	return 0;
}

uint64_t SceneManager::p_getSceneIndex(Scene* scene) {
	const uint64_t max = m_scenes.size();
	for(uint64_t i = 0; i < max; i++)
		if(getScene(i) == scene)
			return i;
	XN_LOG_ERR("SceneManager: Scene {0} is not loaded.", scene);
	return 0;
}


}
