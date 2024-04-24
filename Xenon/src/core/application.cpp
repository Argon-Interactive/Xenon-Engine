#include <glad.h>
#include "application.h"
#include "globalData.hpp"
#include "appData.h"
#include "logger.hpp"
#include "shader.h"

namespace Xenon
{
	Application::Application() {
		XN_LOG_INF("app created");
		if (glfwInit() == GLFW_FALSE)
		{
			XN_LOG_ERR("Failed to initialize GLFW"); exit(EXIT_FAILURE);
		}
		Core::AppData::init(800, 600, "ta rakieta");
		Core::Shader::enableBlending();
		int fragTextureSlots;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &fragTextureSlots);
		Core::Global::fragmentTextureSlots = static_cast<unsigned int>(fragTextureSlots);
	}

	Application::~Application() {
		XN_LOG_INF("app destroyed");
		Core::AppData::terminate();
		glfwTerminate();
	}

	void Application::run() {
		while (!Core::AppData::getWindow().closeCallBack()) {
			// aplikacja dzia¿a
			Core::AppData::getWindow().FEP();
		}
	}

}
