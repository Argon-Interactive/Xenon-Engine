#include <iostream>

#include "xenon.h"

class Sandbox : public Xenon::Application
{
public:
	Sandbox() {

	}
	~Sandbox() override {

	}
};

Xenon::Application* Xenon::create() {
	return new Sandbox();
}