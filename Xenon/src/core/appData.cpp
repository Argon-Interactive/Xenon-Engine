#include "appData.hpp"
#include "Xenon/appConfig.hpp"
#include "input/input.hpp"
#include "devTools/logger_core.hpp"

namespace Core {

void AppData::init(std::function<void(const Event&)> eventDispatcher, const Xenon::AppConfig& config) {
	if(s_exists) {
		XN_LOG_ERR("AppData: Attempted to initialize already initialized AppData!!!");
		return;
	}

	XN_LOG_BR();
	XN_LOG_TRC("AppData: Application Systems initialization...");

	if (glfwInit() == GLFW_FALSE) {
		XN_LOG_ERR("AppData: Failed to initialize GLFW"); 
		exit(EXIT_FAILURE);
	}

	s_appData = std::make_unique<AppData>(ConstructorToken{}, config);
	getWindow().setEventDispatcher(eventDispatcher);

	Input::init(getWindow().passPointer());

	XN_LOG_INF("AppData: Application systems initialized successfully");
	XN_LOG_BR();

	XN_LOG_INF("AppData: Initializing game");
	s_appData->getSceneManager().loadScene(0);
	XN_LOG_BR();
}

void AppData::terminate() {

	XN_LOG_BR();
	XN_LOG_TRC("AppData: Game cleanup...");

	s_appData->getSceneManager().purge();

	XN_LOG_TRC("AppData: Game clened up successfully");

	XN_LOG_BR();
	XN_LOG_TRC("AppData: Application systems termination...");

	s_appData.reset();

	XN_LOG_WAR("AppData: Calling glfw terminate");
	glfwTerminate();

	XN_LOG_INF("AppData: Application systems terminated successfully");
	XN_LOG_BR();
}

AppData::AppData([[maybe_unused]]ConstructorToken t, const Xenon::AppConfig& appConfig)
	:m_window(appConfig.defaultWindowWidth, appConfig.defaultWindowHeight, appConfig.defaultWindowName) {
	s_exists = true;
}

Window& AppData::getWindow() { return s_appData->m_window; }
SceneManager& AppData::getSceneManager() { return s_appData->m_sceneManager; }
ComponentManager& AppData::getComponentManager() { return s_appData->m_componentManager; }
AssetsManager& AppData::getAssetManager() { return s_appData->m_assetManager; }

}
