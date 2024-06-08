#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "scene.hpp"
#include <memory>

namespace Core {

class SceneManager {
public:
	SceneManager();
	~SceneManager();
	SceneManager(SceneManager&&) = delete;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	
	template<typename T>
	void loadScene() {
		m_currentScene = std::make_unique<T>();
	}


private:
	std::unique_ptr<Xenon::Scene> m_currentScene; //TODO a scene could probably be on the stack since it will hold all it's data on the heap anyways	

};

}

#endif // !DEBUG

