#include "xenon.h"

void config([[maybe_unused]]void* something) {
	XN_LOG_DEB("Helo, I am under the water");
}

void scene1(Xenon::Scene* scene) {
	scene->setBuildIndex(0);
	XN_LOG_DEB(scene->getBuildIndex());
}

std::function<void(void*)> defineConfig() {
	return config;
}

std::vector<std::function<void(Xenon::Scene*)>> defineScenes() {
	return {scene1};
}
