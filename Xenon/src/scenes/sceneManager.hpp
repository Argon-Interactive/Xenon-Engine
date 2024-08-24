#ifndef _XENON_SCENES_SCENEMANAGER_
#define _XENON_SCENES_SCENEMANAGER_

#include "scene.hpp"

#include <cstdint>
#include <list>
#include <memory>
#include <vector>
#include <mutex>
#include <future>

namespace Core {

class SceneManager {
public:
	~SceneManager();
	SceneManager(SceneManager&&) = delete;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	
	Scene* createScene();
	void purge();
	void purgeAsync();
	[[nodiscard]] Scene* getScene(uint64_t index) const;
	[[nodiscard]] Scene* getSceneByBuildIndex(uint64_t buildIndex) const;
	[[nodiscard]] uint64_t getSceneCount() const;

	void loadScene(uint64_t buildIndex);
	void unloadSceneAt(uint64_t index);
	void unloadScene(Scene* scene);
	void unloadScene(uint64_t buildIndex);

	void loadSceneAsync(uint64_t buildIndex);
	void unloadSceneAsyncAt(uint64_t index);
	void unloadSceneAsync(Scene* scene);
	void unloadSceneAsync(uint64_t buildIndex);

	void switchScene(uint64_t buildIndex);
	void switchSceneImmediate(uint64_t buildIndex);
	void switchSceneAsync(uint64_t buildIndex);

	void setActiveSceneAt(uint64_t index);
	void setActiveScene(Scene* scene);
	void setActiveScene(uint64_t buildIndex);
	[[nodiscard]] Scene* getActiveScene() const;
	[[nodiscard]] uint64_t getActiveSceneIndex() const;

	void moveEntity(Entity entity, uint64_t targetSceneIndex);
	void moveEntity(Entity entity, Scene* targetScene);

private:
	SceneManager();

	constexpr inline static uint64_t s_invalid = std::numeric_limits<uint64_t>::max();

	std::vector<std::unique_ptr<Scene>> m_scenes;
	uint64_t m_activeSceneIndex = 0;
	bool m_closing = false;

	std::list<std::future<void>> m_futures;
	mutable std::mutex m_mutex;
	mutable std::mutex m_futuresMutex;

	void p_unloadScene(uint64_t index);
	void p_deleteScene(uint64_t index);
	std::unique_ptr<Scene> p_popScene(uint64_t index);
	uint64_t p_getSceneIndex(uint64_t buildIndex);
	uint64_t p_getSceneIndex(Scene* scene);
	void p_syncFutures();
	void p_cleanupFutures();
	void close();

	friend class AppData;
};

}

#endif // !_XENON_SCENES_SCENEMANAGER_
