#include <chrono>

#include "xenon.h"
#include "test.h"
#include "xenonDevTools.h"

class Sandbox : public Xenon::Application
{
public:
	Sandbox() {
		Xenon::LogInf("Hello from sandbox");
		Xenon::LogBR(XN_LOG_MODE_INF);
	}
	~Sandbox() override {
		Xenon::LogInf("Bye from sandbox");
	}
};

Xenon::Application* Xenon::create() {
	return new Sandbox();
}