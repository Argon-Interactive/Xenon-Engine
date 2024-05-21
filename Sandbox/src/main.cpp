#include "devTools/logger/logger.hpp"
#include "devTools/logger/logger_client.hpp"
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
};

Xenon::Application* Xenon::create() {
	return new Sandbox();
}
