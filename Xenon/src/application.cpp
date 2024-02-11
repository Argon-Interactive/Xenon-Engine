#include "application.h"

#include "appData.h"
#include "all.hpp"

namespace Xenon
{

	Application::Application() {
		ALL_INF("app created");
		if (glfwInit() == GLFW_FALSE)
		{
			ALL_ERR("Failed to initialize GLFW"); exit(EXIT_FAILURE);
		}
		core::AppData::init(800, 600, "ta rakieta");
	}

	Application::~Application() {
		ALL_INF("app destroyed");
		core::AppData::terminate();
		glfwTerminate();
	}

	void Application::run() {
		while (!core::AppData::getWindow().closeCallBack()) {
			// aplikacja dzia³¹
			core::AppData::getWindow().FEP();
		}
	}

}