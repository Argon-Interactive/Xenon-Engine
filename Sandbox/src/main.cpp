#include <chrono>

#include "xenon.h"
#include "xenonDevTools.h"

class Sandbox : public Xenon::Application
{
public:
	Sandbox() {
		Xenon::LogInf("Hello from sandbox");
		Xenon::LogBR();
	}
	~Sandbox() override {
		Xenon::LogInf("Bye from sandbox");
	}
};

Xenon::Application* Xenon::create() {
	return new Sandbox();
}