#include"test.hpp"
#include"../api.h"
#include"renderer2D_core.hpp"
#include<glad/glad.h>

void Xenon::testRendering() {
	Core::Quad q0(
		{ {0.f, 0.f, 0.f}, {0.f, 0.f}, 0, 255, 255, 255, 255 },
		{ {-100.f, 0.f, 0.f}, {0.f, 0.f}, 0, 255, 255, 255, 255 },
		{ {0.f, -100.f, 0.f}, {0.f, 0.f}, 0, 255, 255, 255, 255 },
		{ {-100.f, -100.f, 0.f}, {0.f, 0.f}, 0, 255, 255, 255, 255 });

	Core::Quad quads[1] = { q0 };

	Core::Texture2D tex("test.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

	std::shared_ptr<Core::Texture2D> texptr = std::make_shared<Core::Texture2D>(tex);

	Xenon::ID testlayer = Core::Renderer2D::getInstance().createStaticLayer(quads, 1, &texptr, 1);

	Core::OrthographicCamera cam(0.f, 0.f, 0.f, 10.f, 10.f);

	Core::Shader testShader("testShader.glsl");

	Core::Renderer2D::getInstance().render(cam, testShader, testlayer);
}