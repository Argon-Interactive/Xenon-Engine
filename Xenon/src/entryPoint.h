#ifndef _XENON_ENTRYPOINT_H_
#define _XENON_ENTRYPOINT_H_

#include "application.h"
#include <memory>

//extern Xenon::Application* Xenon::create();

int main(int argc, char** argv) {
	Xenon::Application* ptr = Xenon::create();
	std::unique_ptr<Xenon::Application> game(ptr);
	game->run();
}


#endif // !_XENON_ENTRYPOINT_H_
