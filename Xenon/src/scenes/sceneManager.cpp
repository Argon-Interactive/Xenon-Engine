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
	m_scenes.emplace_back(std::make_unique<Scene>());
	return m_scenes.back().get();
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
	std::vector<std::unique_ptr<Scene>> toDelete;
	{
		const std::lock_guard<std::mutex> lock(m_mutex);
		if(m_closing) return;
		toDelete = std::move(m_scenes);
		m_activeSceneIndex = 0;
	}
	std::future<void> future = std::async(std::launch::async, [toDelete = std::move(toDelete)]() mutable {
		XN_LOG_TRC("SceneManager: Purging scenes asynchronously");
		while(!toDelete.empty()) {
			toDelete.back()->unload();
			toDelete.pop_back();
		}
		XN_LOG_TRC("SceneManager: Purging scenes complete");
	});
	{
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_futures.push_back(std::move(future));
	}
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
	const uint64_t index = p_getSceneIndex(scene);
	p_unloadScene(index);
}

void SceneManager::unloadScene(uint64_t buildIndex) {
	XN_LOG_TRC("SceneManager: Unloading scene {0}", buildIndex);
	const std::lock_guard<std::mutex> lock(m_mutex);
	if(m_closing) return;
	const uint64_t index = p_getSceneIndex(buildIndex);
	p_unloadScene(index);
}

void SceneManager::loadSceneAsync([[maybe_unused]] uint64_t buildIndex) {
	{
		const std::lock_guard<std::mutex> lock(m_mutex);
		if(m_closing) return;
	}
	std::future<void> future = std::async(std::launch::async, [this, buildIndex]() {
		XN_LOG_TRC("SceneManager: Loading scene {0} asynchronously", buildIndex);
		auto scene = std::make_unique<Scene>(buildIndex);
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			m_scenes.push_back(std::move(scene));
		}
	});
	{
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_futures.push_back(std::move(future));
		p_cleanupFutures();
	}
}

void SceneManager::unloadSceneAsyncAt([[maybe_unused]] uint64_t index) {
	{
		const std::lock_guard<std::mutex> lock(m_mutex);
		if(m_closing) return;
	}
	std::future<void> future = std::async(std::launch::async, [this, index]() {
		XN_LOG_TRC("SceneManager: Unloading scene at {0} asynchronously", index);
		std::unique_ptr<Scene> s;
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			s = p_popScene(index);
		}
	});
	{
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_futures.push_back(std::move(future));
		p_cleanupFutures();
	}
}

void SceneManager::unloadSceneAsync([[maybe_unused]] Scene* scene) {
	{
		const std::lock_guard<std::mutex> lock(m_mutex);
		if(m_closing) return;
	}
	std::future<void> future = std::async(std::launch::async, [this, scene]() {
		XN_LOG_TRC("SceneManager: Unloading scene {0} asynchronously", scene);
		std::unique_ptr<Scene> s;
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			s = p_popScene(p_getSceneIndex(scene));
		}
	});
	{
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_futures.push_back(std::move(future));
		p_cleanupFutures();
	}
}

void SceneManager::unloadSceneAsync([[maybe_unused]] uint64_t buildIndex) {
	{
		const std::lock_guard<std::mutex> lock(m_mutex);
		if(m_closing) return;
	}
	std::future<void> future = std::async(std::launch::async, [this, buildIndex]() {
		XN_LOG_TRC("SceneManager: Unloading scene {0} asynchronously", buildIndex);
		std::unique_ptr<Scene> s;
		{
			const std::lock_guard<std::mutex> lock(m_mutex);
			s = p_popScene(p_getSceneIndex(buildIndex));
		}
	});
	{
		const std::lock_guard<std::mutex> lock(m_futuresMutex);
		m_futures.push_back(std::move(future));
		p_cleanupFutures();
	}
}

void SceneManager::switchScene(uint64_t buildIndex) {
	p_syncFutures();
	purge();
	loadScene(buildIndex);
}

void SceneManager::switchSceneImmediate(uint64_t buildIndex) {
	p_syncFutures();
	purgeAsync();
	loadScene(buildIndex);
}

void SceneManager::switchSceneAsync(uint64_t buildIndex) {
	p_syncFutures();
	{
		const std::lock_guard<std::mutex> lock(m_mutex);
		if(m_closing) return;
	}
	std::future<void> future = std::async(std::launch::async, [this, buildIndex]() {
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
	if(index == s_invalid) [[unlikely]] return;
	if(index == m_scenes.size() - 1)
		m_activeSceneIndex--;
	getScene(index)->unload();
	p_deleteScene(index);
}

void SceneManager::p_deleteScene(uint64_t index) {
	if(index >= m_scenes.size()) [[unlikely]] {
		XN_LOG_ERR("SceneManager: Trying to delete scene at {0}. It does not exist", index);
		return;
	}
	m_scenes.erase(m_scenes.begin() + static_cast<int64_t>(index));
}

std::unique_ptr<Scene> SceneManager::p_popScene(uint64_t index) {
	if(index >= m_scenes.size()) [[unlikely]] {
		return nullptr;
	}
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
	return s_invalid;
}

uint64_t SceneManager::p_getSceneIndex(Scene* scene) {
	const uint64_t max = m_scenes.size();
	for(uint64_t i = 0; i < max; i++)
		if(getScene(i) == scene)
			return i;
	XN_LOG_ERR("SceneManager: Scene {0} is not loaded.", scene);
	return s_invalid;
}

void SceneManager::p_syncFutures() {
	const std::lock_guard<std::mutex> lock(m_futuresMutex);
	for (auto &future : m_futures) {
		future.get();
	};
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
		const std::lock_guard<std::mutex> lock1(m_futuresMutex);
		const std::lock_guard<std::mutex> lock2(m_mutex);
		m_closing = true;
		for (auto &future : m_futures) {
			future.wait();
		};
	} catch (std::exception& e) {
		try {
			XN_LOG_ERR("SceneManager: Exception in SceneManager::close(): {0}", e.what());
		} catch (std::exception& e2) { 
			exit(-1); // This is never gonna happen basically
		}
	}
}



}
