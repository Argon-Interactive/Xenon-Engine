#ifndef _XENON_CORE_APPDATA_
#define _XENON_CORE_APPDATA_

#include "event.hpp"
#include "scenes/sceneManager.hpp"
#include "ECS/componentManager.hpp"
#include "window/window.hpp"
#include "core/resourcesManagment/resourceManager.hpp"
#include "Xenon/appConfig.hpp"

#include <functional>
#include <memory>

namespace Core {

class AppData {
private:
	struct ConstructorToken {};

public:
	AppData(ConstructorToken t, const Xenon::AppConfig& appConfig);

	static void init(std::function<void(const Event&)> eventDispatcher, const Xenon::AppConfig& config);
	static void terminate();

	[[nodiscard]] static Core::Window& getWindow();
	[[nodiscard]] static Core::SceneManager& getSceneManager();
	[[nodiscard]] static Core::ComponentManager& getComponentManager();
	[[nodiscard]] static Core::ResourceManager& getResourceManager();

private:

	inline static std::unique_ptr<AppData> s_appData;
	inline static bool s_exists;	// XD

	Core::Window m_window;
	Core::SceneManager m_sceneManager;
	Core::ComponentManager m_componentManager;
	Core::ResourceManager m_resourceManager;
};

}

#endif // !_XENON_CORE_APPDATA_
