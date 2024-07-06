#ifndef _XENON_XENON_ENTRYPOINT_
#define _XENON_XENON_ENTRYPOINT_

#include "application.hpp"
#include "logger.hpp"
#include <memory>

std::unique_ptr<Xenon::Application> create();

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	XNTools::Logger::getInstance().setName("[CLIENT]");
	std::unique_ptr<Xenon::Application> game(create());
	int res = game->run();
	return res;
}

#endif // !_XENON_XENON_ENTRYPOINT_
