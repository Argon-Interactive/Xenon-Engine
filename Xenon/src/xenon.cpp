#include "xenon.h"
#include <all.hpp>
#include "window.h"

namespace Xenon
{
	void testMe() {
		ALL_ENT("Welcome in Xenon Engine");
	}
	void runApp(const char* name) {
		if (glfwInit() == GLFW_FALSE)
		{ ALL_ERR("Failed to initialize GLFW"); exit(EXIT_FAILURE); }
		core::Window window(480, 480, name);
		while (!window.closeCallBack())
		{
			window.FEP();
		}
		glfwTerminate();
	}
}
