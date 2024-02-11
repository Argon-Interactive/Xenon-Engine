#define XENON_DLL_BUILD
#include "application.h"

#include "all.hpp"

namespace Xenon
{

	Application::Application() {
		ALL_INF("app created");
	}

	Application::~Application() {
		ALL_INF("app destroyed");
	}

	void Application::run() {
		ALL_INF("app run");
		ALL_INF("1");
		ALL_INF("2");
		ALL_INF("3");
		ALL_INF("app end");
	}

}