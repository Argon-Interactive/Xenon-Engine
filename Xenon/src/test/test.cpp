#include"test.hpp"
#include"../api.h"
#include"renderer2D_core.hpp"
#include<glad.h>
#include"appData.h"
#include"logger.hpp"

void Xenon::testRendering() {
	Core::Quad q0(
		{ {-50.f, 50.f, 1.f}, {0.f, 0.f}, {255.f, 0.f, 255.f, 255.f}, 0,},
		{ {50.f, 50.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 0.f, 255.f}, 0,},
		{ {-50.f, -50.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0,},
		{ {50.f, -50.f, 1.f}, {0.f, 0.f}, {0.f, 255.f, 255.f, 255.f}, 0 });

	Core::Quad q1(
		{ {65.f, 80.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0,},
		{ {80.f, 80.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0,},
		{ {65.f, 65.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0,},
		{ {80.f, 65.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0 });
	
	Core::Quad quads[2] = { q0, q1 };
	
	//Core::Texture2D tex("test.png", 0, 0, 0, 0);
	//std::shared_ptr<Core::Texture2D> texptr = std::make_shared<Core::Texture2D>();

	
	Core::Shader testShader("testShader.glsl");
	Core::OrthographicCamera cam(-50.f, -100.f, 0.f, 200.f, 200.f);
	
	uint32_t test = 0xFFDDCCAA;

	float r = ((test >> 24) & 0xFF);
	float g = ((test >> 16) & 0xFF);
	float b = ((test >> 8) & 0xFF);
	float a = (test & 0xFF);

	XN_LOG_WAR(r, g, b, a);

	Xenon::ID testlayer = Core::Renderer2D::getInstance().createStaticLayer(quads, 2, 1);
	Core::Renderer2D::getInstance().render(cam, testShader, testlayer);

	while (!Core::AppData::getWindow().closeCallBack()) {
		Core::Renderer2D::getInstance().render(cam, testShader, testlayer);
		Core::AppData::getWindow().FEP();
	}
}