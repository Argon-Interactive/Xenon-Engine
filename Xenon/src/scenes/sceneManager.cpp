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
	XN_LOG_INF("SceneManager: Creating scene");
	const std::lock_guard<std::mutex> lock(m_mutex);
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
	XN_LOG_INF("SceneManager: Loading scene {0}", buildIndex);
	const std::lock_guard<std::mutex> lock(m_mutex);
	m_scenes.emplace_back(std::make_unique<Scene>(buildIndex));
}

void SceneManager::unloadSceneAt(uint64_t index) {
	XN_LOG_INF("SceneManager: Unloading scene at {0}", index);
	const std::lock_guard<std::mutex> lock(m_mutex);
	p_unloadScene(index);
}

void SceneManager::unloadScene(Scene* scene) {
	XN_LOG_INF("SceneManager: Unloading scene {0}", scene);
	const std::lock_guard<std::mutex> lock(m_mutex);
	const uint64_t index = p_getSceneIndex(scene);
	p_unloadScene(index);
}

void SceneManager::unloadScene(uint64_t buildIndex) {
	XN_LOG_INF("SceneManager: Unloading scene {0}", buildIndex);
	const std::lock_guard<std::mutex> lock(m_mutex);
	const uint64_t index = p_getSceneIndex(buildIndex);
	p_unloadScene(index);
}

void SceneManager::loadSceneAsync([[maybe_unused]] uint64_t buildIndex) {
	std::future<void> future = std::async(std::launch::async, [this, buildIndex]() {
		XN_LOG_INF("SceneManager: Loading scene {0} asynchronously", buildIndex);
		auto scene = std::make_unique<Scene>(buildIndex);
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			m_scenes.push_back(std::move(scene));
		}
	});
	{
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_futures.push_back(std::move(future));
	}
}

void SceneManager::unloadSceneAsyncAt([[maybe_unused]] uint64_t index) {
	std::future<void> future = std::async(std::launch::async, [this, index]() {
		XN_LOG_INF("SceneManager: Unloading scene at {0} asynchronously", index);
		std::unique_ptr<Scene> s;
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			s = p_popScene(index);
		}
	});
	{
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_futures.push_back(std::move(future));
	}
}

void SceneManager::unloadSceneAsync([[maybe_unused]] Scene* scene) {
	std::future<void> future = std::async(std::launch::async, [this, scene]() {
		XN_LOG_INF("SceneManager: Unloading scene {0} asynchronously", scene);
		std::unique_ptr<Scene> s;
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			s = p_popScene(p_getSceneIndex(scene));
		}
	});
	{
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_futures.push_back(std::move(future));
	}
}

void SceneManager::unloadSceneAsync([[maybe_unused]] uint64_t buildIndex) {
	std::future<void> future = std::async(std::launch::async, [this, buildIndex]() {
		XN_LOG_ERR("SceneManager: Unloading scene {0} asynchronously", buildIndex);
		std::unique_ptr<Scene> s;
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			s = p_popScene(p_getSceneIndex(buildIndex));
		}
	});
	{
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_futures.push_back(std::move(future));
	}
}

/*------------------------------------------------------------------------------------------------*/
/*                                          Active scene                                          */
/*------------------------------------------------------------------------------------------------*/

void SceneManager::setActiveSceneAt(uint64_t index) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	m_activeSceneIndex = index;
}

void SceneManager::setActiveScene(Scene* scene) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	m_activeSceneIndex = p_getSceneIndex(scene);
}

void SceneManager::setActiveScene(uint64_t buildIndex) {
	const std::lock_guard<std::mutex> lock(m_mutex);
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

void SceneManager::p_unloadScene(uint64_t index) {
	if(index == m_scenes.size() - 1)
		m_activeSceneIndex--;
	getScene(index)->unload();
	p_deleteScene(index);
}

void SceneManager::p_deleteScene(uint64_t index) {
	m_scenes.erase(m_scenes.begin() + static_cast<int64_t>(index));
}

std::unique_ptr<Scene> SceneManager::p_popScene(uint64_t index) {
	auto it = m_scenes.begin() + static_cast<int64_t>(index);
	auto scene = std::move(*it);
	m_scenes.erase(it);
	return scene;
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
