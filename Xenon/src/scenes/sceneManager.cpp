#include "sceneManager.hpp"
#include "devTools/logger_core.hpp"
#include "Xenon/scene.hpp"

#include <cstdint>

namespace Core {

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

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

void SceneManager::loadScene(uint64_t buildIndex) {
	m_scenes.emplace_back(std::make_unique<Scene>(buildIndex));
	XN_LOG_DEB("Load the scene here. Scene build index: {0}", buildIndex);
	m_scenes.back()->setBuildIndex(buildIndex);
}

void SceneManager::unloadScene(uint64_t buildIndex) {
	const uint64_t index = findNearestScene(buildIndex);
	if(index == m_scenes.size() - 1)
		m_activeSceneIndex--;
	getScene(index)->unload();
	deleteScene(index);
}

void SceneManager::loadSceneAsync([[maybe_unused]] uint64_t buildIndex) {
	XN_LOG_ERR("Do not use SceneManager::loadSceneAsync. This method does not have any implementation.");
}

void SceneManager::unloadSceneAsync([[maybe_unused]] uint64_t buildIndex) {
	XN_LOG_ERR("Do not use SceneManager::unloadSceneAsync. This method does not have any implementation.");
}

void SceneManager::setActiveScene(uint64_t buildIndex) {
	m_activeSceneIndex = findNearestScene(buildIndex);
}

Scene* SceneManager::getActiveScene() {
	return m_scenes[m_activeSceneIndex].get();
}

Scene* SceneManager::getScene(uint64_t index) {
	return m_scenes[index].get();
}

Scene* SceneManager::getSceneByBuildIndex(uint64_t buildIndex) {
	for(auto& scene : m_scenes)
		if(scene->getBuildIndex() == buildIndex)
			return scene.get();
	XN_LOG_ERR("Scene with build index {0} is not currently loaded.", buildIndex);
	return nullptr;
}

uint64_t SceneManager::getActiveSceneIndex() const {
	return m_activeSceneIndex;
}

uint64_t SceneManager::getSceneCount() const {
	 return m_scenes.size();
}



void SceneManager::deleteScene(uint64_t index) {
	m_scenes.erase(m_scenes.begin() + static_cast<int64_t>(index));
}

uint64_t SceneManager::findNearestScene(uint64_t buildIndex) {
	const uint64_t max = getSceneCount();
	for(uint64_t i = 0; i < max; i++)
		if(getScene(i)->getBuildIndex() == buildIndex)
			return i;
	XN_LOG_ERR("Scene of build index {0} is not loaded.", buildIndex);
	return 0;
}


}
