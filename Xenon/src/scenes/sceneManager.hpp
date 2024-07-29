#ifndef _XENON_SCENES_SCENEMANAGER_
#define _XENON_SCENES_SCENEMANAGER_

#include "Xenon/scene.hpp"
#include "scene.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

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
	void loadScene(uint64_t buildIndex);
	void unloadScene(uint64_t buildIndex);
	void loadSceneAsync(uint64_t buildIndex);
	void unloadSceneAsync(uint64_t buildIndex);
	void setActiveScene(uint64_t buildIndex);
	[[nodiscard]] Scene* getActiveScene();
	[[nodiscard]] Scene* getScene(uint64_t index);
	[[nodiscard]] Scene* getSceneByBuildIndex(uint64_t buildIndex);
	[[nodiscard]] uint64_t getActiveSceneIndex() const;

	[[nodiscard]] uint64_t getSceneCount() const;

private:
	explicit SceneManager(std::vector<std::function<void(Xenon::Scene*)>> buildFunctions);

	std::vector<std::unique_ptr<Scene>> m_scenes;
	uint64_t m_activeSceneIndex = 0;
	uint64_t m_loadedSceneCount = 0;

	std::vector<std::function<void(Xenon::Scene*)>> m_buildFunctions;

	void deleteScene(uint64_t index);
	uint64_t findNearestScene(uint64_t buildIndex);

	friend class AppData;
};

}

#endif // _XENON_SCENES_SCENEMANAGER_
