#define XENON_DEBUG //This macro makes Xenon run the application in debug mode

#include "xenon.h"

void config([[maybe_unused]]void* something) {
	XN_LOG_BR(XNTools::Logger::logMode::info);
	XN_LOG_DEB("Helo, I am under the water");
}

void scene1(Xenon::Scene* scene) {
	XN_LOG_DEB(scene->getBuildIndex());
	scene->createEntity();
	Core::Test::test();
}

std::function<void(void*)> defineConfig() {
	return config;
}

std::vector<std::function<void(Xenon::Scene*)>> defineScenes() {
	return {scene1};
}
