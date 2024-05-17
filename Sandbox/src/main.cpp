#include "devTools/logger/logger.hpp"
#include "devTools/logger/logger_client.hpp"
#include "xenon.h"

class Sandbox : public Xenon::Application
{
public:
	Sandbox() {
		XN_LOG_ENT("Hello from sandbox");
		XN_LOG_BR(Xenon::Logger::logMode::info);

		XN_LOG_DEB("logger test");
		XN_LOG_TRC("logger test");
		XN_LOG_ENT("logger test");
		XN_LOG_INF("logger test");
		XN_LOG_WAR("logger test");
		XN_LOG_ERR("logger test");
	}
	~Sandbox() override {
		XN_LOG_ENT("Bye from sandbox");
	}
};

Xenon::Application* Xenon::create() {
	return new Sandbox();
}
