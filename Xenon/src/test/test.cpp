#include"test.hpp"
#include"../api.h"
#include"renderer2D_core.hpp"
#include<glad.h>
#include"appData.h"
#include"logger.hpp"

void Xenon::testRendering() {
	Core::Quad q0(
		{ {-50.f, 50.f, 1.f }, {0.f, 0.f}, {255.f, 0.f, 255.f, 255.f  }, 0,},
		{ {50.f, 50.f, 1.f  }, {0.f, 0.f}, {255.f, 255.f, 0.f, 255.f  }, 0,},
		{ {-50.f, -50.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0,},
		{ {50.f, -50.f, 1.f }, {0.f, 0.f}, {0.f, 255.f, 255.f, 255.f  }, 0 });

	Core::Quad q1(
		{ {65.f, 80.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0,},
		{ {80.f, 80.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0,},
		{ {65.f, 65.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0,},
		{ {80.f, 65.f, 1.f}, {0.f, 0.f}, {255.f, 255.f, 255.f, 255.f}, 0 });
	
	Core::Quad quads[2] = { q0, q1 };
	
	std::shared_ptr<Core::Texture2D> texptr = std::make_shared<Core::Texture2D>("test.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

	Core::Shader testShader("testShader.glsl");
	Core::OrthographicCamera cam(-50.f, -100.f, 0.f, 200.f, 200.f);

	Xenon::ID testlayer = Core::Renderer2D::getInstance().createStaticLayer(quads, 2, &texptr, 1);
	Core::Renderer2D::getInstance().render(cam, testShader, testlayer);

	while (!Core::AppData::getWindow().closeCallBack()) {
		Core::Renderer2D::getInstance().render(cam, testShader, testlayer);
		Core::AppData::getWindow().FEP();
	}
}