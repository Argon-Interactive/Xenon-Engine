#ifndef _XENON_XENON_ENTRYPOINT_
#define _XENON_XENON_ENTRYPOINT_

#include "application.hpp"
#include "scene.hpp"

#include <vector>
#include <functional>

std::function<void(void*)> defineConfig();
std::vector<std::function<void(Xenon::Scene*)>> defineScenes();

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	Xenon::Application game;
	game.addConfiguration(defineConfig());
	game.addScenes(defineScenes());
	game.create();
	int res = game.run();
	return res;
}

#endif // !_XENON_XENON_ENTRYPOINT_
