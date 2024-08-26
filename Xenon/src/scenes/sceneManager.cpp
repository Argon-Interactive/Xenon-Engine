#include "sceneManager.hpp"
#include "devTools/logger_core.hpp"
#include "Xenon/scene.hpp"

#include <cstdint>

namespace Core {

SceneManager::SceneManager() = default;

SceneManager::~SceneManager() {
	close();
};

/*------------------------------------------------------------------------------------------------*/
/*                                         Basic Methods                                          */
/*------------------------------------------------------------------------------------------------*/

Scene* SceneManager::createScene() {
	XN_LOG_TRC("SceneManager: Creating scene");
	const std::lock_guard<std::mutex> lock(m_mutex);
	if(m_closing) return nullptr;
	m_activeSceneIndex = m_scenes.size();
	return m_scenes.emplace_back(std::make_unique<Scene>()).get();
}

void SceneManager::purge() {
	p_syncFutures();
	XN_LOG_TRC("SceneManager: Purging scenes");
	const std::lock_guard<std::mutex> lock(m_mutex);
	if(m_closing) return;
	while(!m_scenes.empty()) {
		m_scenes.back()->unload();
		m_scenes.pop_back();
	}
	m_activeSceneIndex = 0;
	XN_LOG_TRC("SceneManager: Purging scenes complete");
}

void SceneManager::purgeAsync() {
	p_syncFutures();
	const std::lock_guard<std::mutex> lock(m_mutex);
	const std::lock_guard<std::mutex> lockf(m_futuresMutex);
	if(m_closing) return;
	m_activeSceneIndex = 0;
	m_futures.push_back(std::async(std::launch::async, [toDelete = std::move(m_scenes)]() mutable {
		XN_LOG_TRC("SceneManager: Purging scenes asynchronously");
		while(!toDelete.empty()) {
			toDelete.back()->unload();
			toDelete.pop_back();
		}
		XN_LOG_TRC("SceneManager: Purging scenes complete");
	}));
}

Scene* SceneManager::getScene(uint64_t index) const {
	const std::lock_guard<std::mutex> lock(m_mutex);
	return m_scenes[index].get();
}

Scene* SceneManager::getSceneByBuildIndex(uint64_t buildIndex) const {
	const std::lock_guard<std::mutex> lock(m_mutex);
	for(const auto& scene : m_scenes)
		if(scene->getBuildIndex() == buildIndex)
			return scene.get();
	XN_LOG_ERR("SceneManager: Scene with build index {0} is not currently loaded.", buildIndex);
	return nullptr;
}

uint64_t SceneManager::getSceneCount() const {
	const std::lock_guard<std::mutex> lock(m_mutex);
	 return m_scenes.size();
}

/*------------------------------------------------------------------------------------------------*/
/*                                       Loading/Unloading                                        */
/*------------------------------------------------------------------------------------------------*/

void SceneManager::loadScene(uint64_t buildIndex) {
	XN_LOG_TRC("SceneManager: Loading scene {0}", buildIndex);
	const std::lock_guard<std::mutex> lock(m_mutex);
	if(m_closing) return;
	m_activeSceneIndex = m_scenes.size();
	m_scenes.emplace_back(std::make_unique<Scene>(buildIndex));
}

void SceneManager::unloadSceneAt(uint64_t index) {
	XN_LOG_TRC("SceneManager: Unloading scene at {0}", index);
	const std::lock_guard<std::mutex> lock(m_mutex);
	if(m_closing) return;
	p_unloadScene(index);
}

void SceneManager::unloadScene(Scene* scene) {
	XN_LOG_TRC("SceneManager: Unloading scene {0}", scene);
	const std::lock_guard<std::mutex> lock(m_mutex);
	if(m_closing) return;
	p_unloadScene(p_getSceneIndex(scene));
}

void SceneManager::unloadScene(uint64_t buildIndex) {
	XN_LOG_TRC("SceneManager: Unloading scene {0}", buildIndex);
	const std::lock_guard<std::mutex> lock(m_mutex);
	if(m_closing) return;
	p_unloadScene(p_getSceneIndex(buildIndex));
}

void SceneManager::loadSceneAsync(uint64_t buildIndex) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	const std::lock_guard<std::mutex> lockf(m_futuresMutex);
	if(m_closing) return;
	m_futures.push_back(std::async(std::launch::async, [this, buildIndex]() {
		XN_LOG_TRC("SceneManager: Loading scene {0} asynchronously", buildIndex);
		auto scene = std::make_unique<Scene>(buildIndex);
		const std::lock_guard<std::mutex> lock(m_mutex);
		m_activeSceneIndex = m_scenes.size();
		m_scenes.push_back(std::move(scene));
	}));
	p_cleanupFutures();
}

void SceneManager::unloadSceneAsyncAt(uint64_t index) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	const std::lock_guard<std::mutex> lockf(m_futuresMutex);
	if(m_closing) return;
	m_futures.push_back(std::async(std::launch::async, [this, index]() {
		XN_LOG_TRC("SceneManager: Unloading scene at {0} asynchronously", index);
		std::unique_ptr<Scene> s = p_popScene(index);
	}));
	p_cleanupFutures();
}

void SceneManager::unloadSceneAsync(Scene* scene) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	const std::lock_guard<std::mutex> lockf(m_futuresMutex);
	if(m_closing) return;
	m_futures.push_back(std::async(std::launch::async, [this, scene]() {
		XN_LOG_TRC("SceneManager: Unloading scene {0} asynchronously", scene);
		std::unique_ptr<Scene> s = p_popScene(p_getSceneIndex(scene));
	}));
	p_cleanupFutures();
}

void SceneManager::unloadSceneAsync(uint64_t buildIndex) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	const std::lock_guard<std::mutex> lockf(m_futuresMutex);
	if(m_closing) return;
	m_futures.push_back(std::async(std::launch::async, [this, buildIndex]() {
		XN_LOG_TRC("SceneManager: Unloading scene {0} asynchronously", buildIndex);
		std::unique_ptr<Scene> s = p_popScene(p_getSceneIndex(buildIndex));
	}));
	p_cleanupFutures();
}

void SceneManager::switchScene(uint64_t buildIndex) {
	purge();
	loadScene(buildIndex);
}

void SceneManager::switchSceneImmediate(uint64_t buildIndex) {
	purgeAsync();
	loadScene(buildIndex);
}

void SceneManager::switchSceneAsync(uint64_t buildIndex) {
	p_syncFutures();
	const std::lock_guard<std::mutex> lock(m_mutex);
	const std::lock_guard<std::mutex> lockf(m_futuresMutex);
	if(m_closing) return;
	m_futures.push_back(std::async(std::launch::async, [this, buildIndex]() {
		XN_LOG_TRC("SceneManager: Loading scene {0} asynchronously", buildIndex);
		auto scene = std::make_unique<Scene>(buildIndex);
		std::vector<std::unique_ptr<Scene>> toDelete;
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			toDelete = std::move(m_scenes);
			m_scenes.push_back(std::move(scene));
		}
		XN_LOG_TRC("SceneManager: Purging scenes asynchronously");
		while(!toDelete.empty()) {
			toDelete.back()->unload();
			toDelete.pop_back();
		}
		XN_LOG_TRC("SceneManager: Purging scenes complete");
	}));
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
	const std::lock_guard<std::mutex> lock(m_mutex);
	return m_scenes[m_activeSceneIndex].get();
}

uint64_t SceneManager::getActiveSceneIndex() const {
	const std::lock_guard<std::mutex> lock(m_mutex);
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
	if(index >= m_scenes.size()) [[unlikely]] {
		XN_LOG_ERR("SceneManager: Trying to delete scene at {0}. It does not exist", index);
		return;
	}
	if(index == m_scenes.size() - 1)
		m_activeSceneIndex--;
	m_scenes[index]->unload();
	m_scenes.erase(m_scenes.begin() + static_cast<int64_t>(index));
}

std::unique_ptr<Scene> SceneManager::p_popScene(uint64_t index) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	if(index >= m_scenes.size()) [[unlikely]]
		return nullptr;
	if(index == m_scenes.size() - 1)
		m_activeSceneIndex--;
	auto it = m_scenes.begin() + static_cast<int64_t>(index);
	auto scene = std::move(*it);
	m_scenes.erase(it);
	return scene;
}

uint64_t SceneManager::p_getSceneIndex(uint64_t buildIndex) {
	const uint64_t max = m_scenes.size();
	for(uint64_t i = 0; i < max; i++)
		if(m_scenes[i]->getBuildIndex() == buildIndex)
			return i;
	XN_LOG_ERR("SceneManager: Scene of build index {0} is not loaded.", buildIndex);
	return s_invalid;
}

uint64_t SceneManager::p_getSceneIndex(Scene* scene) {
	const uint64_t max = m_scenes.size();
	for(uint64_t i = 0; i < max; i++)
		if(m_scenes[i].get() == scene)
			return i;
	XN_LOG_ERR("SceneManager: Scene {0} is not loaded.", scene);
	return s_invalid;
}

void SceneManager::p_syncFutures() {
	const std::lock_guard<std::mutex> lock(m_futuresMutex);
	for (auto &future : m_futures) {
		future.get();
	}
	m_futures.clear();
}

void SceneManager::p_cleanupFutures() {
	m_futures.erase(
		std::remove_if(m_futures.begin(), m_futures.end(),
				 [](std::future<void> &future) {
					 return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
				 }),
		m_futures.end());
}

void SceneManager::close() {
	try {
		const std::lock_guard<std::mutex> lockf(m_futuresMutex);
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			m_closing = true;
		}
		for (auto &future : m_futures) {
			future.wait();
		}
	} catch (std::exception& e) {
		try {
			XN_LOG_ERR("SceneManager: Exception in SceneManager::close(): {0}", e.what());
		} catch (std::exception& e2) { 
			exit(-1); // This is never gonna happen basically
		}
	}
}



}
