#include <glad.h>
#include "application.h"
#include "globalData.hpp"
#include "appData.h"
#include "logger.hpp"

namespace Xenon
{

	Application::Application() {
		Core::AppData::init();
		XN_LOG_INF("Application: created");
	}

	Application::~Application() {
		Core::AppData::terminate();
		XN_LOG_INF("Application: destroyed");
	}

	int Application::run() {
		while (!Core::AppData::getWindow().closeCallBack()) {
			// aplikacja dziaua
			Core::AppData::getWindow().FEP();
		}
		return 0;
	}

}
