#ifndef _XENON_XENON_APPLICATION_
#define _XENON_XENON_APPLICATION_

#include "api.h"
#include "scene.hpp"

#include <functional>
#include <vector>

extern int main(int argc, char** argv);

namespace Core { class Application; }

namespace Xenon {

class XAPI Application {
public:
	Application();
	~Application();
	Application(const Application &) = delete;
	Application(Application &&) = delete;
	Application &operator=(const Application &) = delete;
	Application &operator=(Application &&) = delete;

	void create();

	void addConfiguration(std::function<void(void*)> configFunction);
	void addScenes(std::vector<std::function<void(Scene*)>> buildFunctions);

private:
	int run();

	Core::Application* m_impl;

	std::function<void(void*)> m_configFunction;
	std::vector<std::function<void(Scene*)>> m_buildFunctions;

	friend int ::main(int argc, char** argv);
};

}

#endif // !_XENON_XENON_APPLICATION_
