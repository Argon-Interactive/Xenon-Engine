#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "scene.hpp"
#include <memory>

namespace Core { // idk if not xenon

	class Ex : public Xenon::Scene {

	};

class SceneManager {
public:
	SceneManager();
	~SceneManager();
	SceneManager(SceneManager&&) = delete;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	
	static void loadScene(std::unique_ptr<Xenon::Scene> scene);
	static void loadSceneWithTransition(std::unique_ptr<Xenon::Scene> scene /*, Transition*/);
	static void update(double deltaT);
	static void render();

private:
	static std::unique_ptr<Xenon::Scene> m_currentScene;

};

}

#endif // !DEBUG

