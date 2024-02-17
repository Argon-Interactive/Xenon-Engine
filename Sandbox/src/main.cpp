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
		Core::Shader shader("assets/test.glsl");
		Xenon::OrthographicCamera2D cam;
		Xenon::StaticLayer2D layer;
	
		layer.addQuad({ 1.f, 1.f });

		layer.compute();

		Xenon::Renderer2D::draw(shader, cam, layer);
	}
	~Sandbox() override {
		Xenon::LogInf("Bye from sandbox");
	}
};

Xenon::Application* Xenon::create() {
	return new Sandbox();
}