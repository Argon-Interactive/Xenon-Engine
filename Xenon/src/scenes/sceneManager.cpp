#include "sceneManager.hpp"
#include "devTools/logger_core.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace Core {

SceneManager::SceneManager(std::vector<std::function<void(Xenon::Scene*)>> buildFunctions)
	: m_buildFunctions(std::move(buildFunctions)) {}
SceneManager::~SceneManager() = default;

Scene* SceneManager::createScene() {
	m_scenes.emplace_back();
	m_scenes.back() = new Scene;
	return m_scenes.back();
}

void SceneManager::purge() {
	for(auto* scene : m_scenes) {
		scene->unload();
		delete scene;
	}
	m_scenes = {};
}

void SceneManager::loadScene(int64_t buildIndex) {
	m_scenes.emplace_back();
	m_scenes.back() = new Scene;
	auto clientScene = Xenon::Scene(m_scenes.back());
	m_buildFunctions[static_cast<size_t>(buildIndex)](&clientScene);
}

void SceneManager::unloadScene(int64_t buildIndex) {
	int64_t index = findNearestScene(buildIndex);
	if(index != -1) [[likely]] {
		if(static_cast<size_t>(index) == m_scenes.size() - 1)
			m_activeSceneIndex--;
		getScene(index)->unload();
		deleteScene(index);
	} else [[unlikely]] {
		XN_LOG_ERR("Scene of build index {0} is not loaded. Failed to unload.", buildIndex);
	}
}

void SceneManager::loadSceneAsync(int64_t buildIndex) {

}

void SceneManager::unloadSceneAsync(int64_t buildIndex) {

}

void SceneManager::setActiveScene(int64_t buildIndex) {
	m_activeSceneIndex = findNearestScene(buildIndex);
}

Scene* SceneManager::getActiveScene() {
	return m_scenes.at(static_cast<size_t>(m_activeSceneIndex));
}

Scene* SceneManager::getScene(int64_t index) {
	if(index < 0 || static_cast<size_t>(index) >= m_scenes.size()) [[unlikely]]
		XN_LOG_ERR("Invalid scene manager index {0}", index);
	return m_scenes.at(static_cast<size_t>(index));
}

Scene* SceneManager::getSceneByBuildIndex(int64_t buildIndex) {
	for(auto* scene : m_scenes)
		if(scene->getBuildIndex() == buildIndex)
			return scene;
	XN_LOG_ERR("Invalid build index {0}", buildIndex);
	return nullptr;
}

int64_t SceneManager::getActiveSceneIndex() const {
	return m_activeSceneIndex;
}

int64_t SceneManager::getSceneCount() const {
	 return static_cast<int64_t>(m_scenes.size());
}



void SceneManager::deleteScene(int64_t index) {
	assert(index >= 0 && static_cast<size_t>(index) < m_scenes.size());
	delete m_scenes.at(index);
	m_scenes.erase(m_scenes.begin() + index);
}

int64_t SceneManager::findNearestScene(int64_t buildIndex) {
	int64_t max = getSceneCount();
	for(int64_t i = 0; i < max; i++)
		if(getScene(i)->getBuildIndex() == buildIndex)
			return i;
	return -1;
}


}
