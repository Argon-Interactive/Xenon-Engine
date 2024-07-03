#ifndef _XENON_XENON_APPLICATION_
#define _XENON_XENON_APPLICATION_

#include "api.h"
#include <memory>

extern int main(int argc, char** argv);

namespace Core { class Application; }

namespace Xenon {

class XAPI Application {
public:
	Application();
	virtual ~Application();
	Application(const Application&) = delete;
	Application(Application&&) noexcept = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

private:
	int run();

	std::unique_ptr<Core::Application> m_impl;

	friend int ::main(int argc, char** argv);
};

}

#endif // !_XENON_XENON_APPLICATION_
