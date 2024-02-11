#include <iostream>

#include "xenon.h"
#include "all.hpp"

class Sandbox : public Xenon::Application
{
public:
	Sandbox() {
		ALL_INF("Hello from sandbox");
	}
	~Sandbox() override {
		ALL_INF("Bye from sandbox");
	}
};

Xenon::Application* Xenon::create() {
	return new Sandbox();
}