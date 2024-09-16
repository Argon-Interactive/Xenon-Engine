#ifndef _XENON_XENON_APPLICATION_
#define _XENON_XENON_APPLICATION_

#include "api.h"
#include "appConfig.hpp"

extern int main(int argc, char** argv);

namespace Core { class Application; }

namespace Xenon {

class XAPI Application {
public:
	explicit Application(const AppConfig& config);
	~Application();
	Application(const Application &) = delete;
	Application(Application &&) = delete;
	Application &operator=(const Application &) = delete;
	Application &operator=(Application &&) = delete;

private:
	int run();

	Core::Application* m_impl;

	friend int ::main(int argc, char** argv);
};

}

#endif // !_XENON_XENON_APPLICATION_
