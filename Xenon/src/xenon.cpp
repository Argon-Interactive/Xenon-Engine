#include "all.hpp"
#include <iostream>
#include "window.h"

namespace xen
{
	void testMe() {
		ALL_ENT("Welcome in Xenon Engine");
	}
	void runApp(const char* name) {
		if (glfwInit() == GLFW_FALSE)
		{ ALL_ERR("Failed to initialize GLFW"); exit(EXIT_FAILURE); }
		xen::Window window(128, 128, name);
		while (window.closeCallBack())
		{
			//run
		}
		glfwTerminate();
	}
}
