#ifndef _XENON_CORE_APPDATA_H_
#define _XENON_CORE_APPDATA_H_

#include "Xenon/scene.hpp"
#include "event.hpp"
#include "scenes/sceneManager.hpp"
#include "window/window.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace Core {

class AppData {
private:
	struct ConstructorToken {};

public:
	AppData(ConstructorToken t, uint32_t width, uint32_t height, const std::string& title, std::vector<std::function<void(Xenon::Scene*)>> buildFunctions);

	static void init(std::function<void(const Event&)> eventDispatcher, std::function<void(void*)> configFunction, std::vector<std::function<void(Xenon::Scene*)>> buildFunctions);
	static void terminate();

	[[nodiscard]] static Core::Window& getWindow();
	[[nodiscard]] static Core::SceneManager& getSceneManager();

private:

	inline static std::unique_ptr<AppData> s_appData;
	inline static bool s_exists;	// XD

	Core::Window m_window;
	Core::SceneManager m_sceneManager;
};

}

#endif // !_XENON_CORE_APPDATA_H_
