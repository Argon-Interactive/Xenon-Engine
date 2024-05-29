#include <functional>
#include <glad.h>
#include "appData.hpp"
#include "devTools/logger/logger_core.hpp"

namespace Core
{

	void AppData::init(std::function<void(const Event&)> eventDispatcher) {
		if(s_exists) {
			XN_LOG_ERR("Attempted to initialize already initialized AppData!!!");
			return;
		}

		XN_LOG_BR();
		XN_LOG_TRC("Application Systems initialization...");

		if (glfwInit() == GLFW_FALSE) {
			XN_LOG_ERR("Failed to initialize GLFW"); 
			exit(EXIT_FAILURE);
		}
		s_appData = std::make_unique<AppData>(ConstructorToken{}, 800, 600, "XENON APP");
		s_appData->getWindow().setEventDispatcher(eventDispatcher);

		XN_LOG_INF("Application systems initialized successfully");
		XN_LOG_BR();
	}

	void AppData::terminate() {
		XN_LOG_BR();
		XN_LOG_TRC("Application systems termination...");
		
		s_appData.reset();

		XN_LOG_WAR("Calling glfw terminate");
		glfwTerminate();

		XN_LOG_INF("Application systems terminated successfully");
		XN_LOG_BR();
	}
	
	AppData::AppData([[maybe_unused]]ConstructorToken t, uint32_t width, uint32_t height, const std::string& title)
		:m_window(width, height, title) {
		s_exists = true;
	}

	Window& AppData::getWindow() {
		return s_appData->m_window;
	}

}
