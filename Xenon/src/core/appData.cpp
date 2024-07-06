#include <functional>
#include <glad.h>
#include "appData.hpp"
#include "devTools/logger_core.hpp"

namespace Core {

void AppData::init(std::function<void(const Event&)> eventDispatcher, [[maybe_unused]] std::function<void(void*)> configFunction, std::vector<std::function<void(Xenon::Scene*)>> buildFunctions) {
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
	s_appData = std::make_unique<AppData>(ConstructorToken{}, 800, 600, "XENON APP", buildFunctions);
	s_appData->getWindow().setEventDispatcher(eventDispatcher);

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

AppData::AppData([[maybe_unused]]ConstructorToken t, uint32_t width, uint32_t height, const std::string& title, std::vector<std::function<void(Xenon::Scene*)>> buildFunctions)
	:m_window(width, height, title), m_sceneManager(buildFunctions) {
	s_exists = true;
}

Window& AppData::getWindow() {
	return s_appData->m_window;
}

SceneManager& AppData::getSceneManager() {
	return s_appData->m_sceneManager;
}

}
