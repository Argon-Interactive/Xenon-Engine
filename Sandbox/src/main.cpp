#include "xenon.h"

class Sandbox : public Xenon::Application
{
public:
	Sandbox() {
		XN_LOG_ENT("Hello from sandbox");
		XN_LOG_BR(Xenon::Logger::logMode::info);
	}
	~Sandbox() override { 
		XN_LOG_ENT("Bye from sandbox"); 
	}

	Sandbox(const Sandbox &) = delete;
	Sandbox(Sandbox &&) = delete;
	Sandbox &operator=(const Sandbox &) = delete;
	Sandbox &operator=(Sandbox &&) = delete;
};

std::unique_ptr<Xenon::Application> create() {
	return std::make_unique<Sandbox>();
}
