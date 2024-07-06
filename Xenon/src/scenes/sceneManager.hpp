#ifndef _XENON_SCENES_SCENEMANAGER_
#define _XENON_SCENES_SCENEMANAGER_

#include "Xenon/scene.hpp"
#include "scene.hpp"

#include <cstdint>
#include <functional>
#include <vector>

namespace Core {

class SceneManager {
public:
	explicit SceneManager(std::vector<std::function<void(Xenon::Scene*)>> buildFunctions);
	~SceneManager();
	SceneManager(SceneManager&&) = delete;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	
	Scene* createScene();
	void purge();
	void loadScene(int64_t buildIndex);
	void unloadScene(int64_t buildIndex);
	void loadSceneAsync(int64_t buildIndex);
	void unloadSceneAsync(int64_t buildIndex);
	void setActiveScene(int64_t buildIndex);
	[[nodiscard]] Scene* getActiveScene();
	[[nodiscard]] Scene* getScene(int64_t index);
	[[nodiscard]] Scene* getSceneByBuildIndex(int64_t buildIndex);
	[[nodiscard]] int64_t getActiveSceneIndex() const;

	[[nodiscard]] int64_t getSceneCount() const;

private:
	std::vector<Scene*> m_scenes;
	int64_t m_activeSceneIndex = 0;
	int64_t m_loadedSceneCount = 0;

	std::vector<std::function<void(Xenon::Scene*)>> m_buildFunctions;

	void deleteScene(int64_t index);
	int64_t findNearestScene(int64_t buildIndex);
};

}

#endif // _XENON_SCENES_SCENEMANAGER_
