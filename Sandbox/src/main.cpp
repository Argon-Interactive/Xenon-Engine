#define XENON_DEBUG //This macro makes Xenon run the application in debug mode

#include "xenon.h"

#include <string>
#include <array>

// void config([[maybe_unused]]void* something) {
// 	XN_LOG_BR(XNTools::Logger::logMode::info);
// 	XN_LOG_DEB("Helo, I am under the water");
// }

void scene1(Xenon::Scene* scene) {
	XN_LOG_DEB(scene->getBuildIndex());
	scene->createEntity();
	Core::Test::test();
}

const std::array<Xenon::BuildFunction, 1> scenes = {
	&scene1 
};

const std::string name = "Sandbox";

const Xenon::AppConfig config{
	800, 600, name.c_str(), 
	{ scenes.data(), scenes.size() }
};

const Xenon::AppConfig& getConfig() {
	return config;
}
