#ifndef _XENON_ENTRYPOINT_H_
#define _XENON_ENTRYPOINT_H_

#include "core.h"
#include "application.h"
#include <memory>

//extern Xenon::Application* Xenon::create();

int main(int argc, char** argv) {
	std::unique_ptr<Xenon::Application> game(Xenon::create());
	game->run();
}

#endif // !_XENON_ENTRYPOINT_H_