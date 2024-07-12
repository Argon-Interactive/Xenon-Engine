#ifndef _XENON_CORE_APPDATA_H_
#define _XENON_CORE_APPDATA_H_

#include "event.hpp"
#include "scenes/sceneManager.hpp"
#include "ECS/componentManager.hpp"
#include "window/window.hpp"
#include "Xenon/appConfig.hpp"

#include <functional>
#include <memory>

namespace Core {

class AppData {
private:
	struct ConstructorToken {};

public:
	AppData(ConstructorToken t, uint32_t width, uint32_t height, const std::string& title, const Xenon::BuildFunctions& buildFunctions);

	static void init(std::function<void(const Event&)> eventDispatcher, const Xenon::AppConfig& config);
	static void terminate();

	[[nodiscard]] static Core::Window& getWindow();
	[[nodiscard]] static Core::SceneManager& getSceneManager();
	[[nodiscard]] static Core::ComponentManager& getComponentManager();

private:

	inline static std::unique_ptr<AppData> s_appData;
	inline static bool s_exists;	// XD

	Core::Window m_window;
	Core::SceneManager m_sceneManager;
	Core::ComponentManager m_componentManager;
};

}

#endif // !_XENON_CORE_APPDATA_H_
