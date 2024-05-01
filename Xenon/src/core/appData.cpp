#include <glad.h>
#include "appData.hpp"
#include "logger.hpp"
#include "shader.hpp"
#include "globalData.hpp"

namespace Core
{

	std::unique_ptr<AppData> AppData::s_appData;
	bool AppData::s_exists = false;

	void AppData::init() {
		XN_LOG_BR;
		XN_LOG_ENT("Application Systems initialization...");

		if (glfwInit() == GLFW_FALSE) {
			XN_LOG_ERR("Failed to initialize GLFW"); 
			exit(EXIT_FAILURE);
		}
		s_appData = std::make_unique<AppData>(800, 600, "XENON APP");

		Core::Shader::enableBlending();		// TODO: Find a better place for this

		int fragTextureSlots;				// Wow this is super specific... does it have to be in global data like this?
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &fragTextureSlots);
		Core::Global::fragmentTextureSlots = static_cast<unsigned int>(fragTextureSlots);

		XN_LOG_INF("Application systems initialized successfully");
		XN_LOG_BR;
	}

	void AppData::terminate() {
		XN_LOG_BR;
		XN_LOG_ENT("Application systems termination...");
		
		s_appData.reset();

		XN_LOG_WAR("Calling glfw terminate");
		glfwTerminate();

		XN_LOG_INF("Application systems terminated successfully");
		XN_LOG_BR;
	}
	
	AppData::AppData(uint32_t width, uint32_t height, const std::string& title)
		:m_window(width, height, title) {
		if(s_exists) {
			XN_LOG_ERR("Creating another instance of AppData! NO FUCKING CLUE HOW TO FORBID THIS");
		}
	}

	Window& AppData::getWindow() {
		return s_appData->m_window;
	}

}
