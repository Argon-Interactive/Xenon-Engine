#ifndef _XENON_APPLICATION_H_
#define _XENON_APPLICATION_H_

#include "../api.h"

extern int main(int argc, char** argv);

namespace Xenon
{

	class XAPI Application
	{
	public:
		Application();
		virtual ~Application();

	private:
		int run();
		void handleEvents();
		void render();
		void update();

		friend int ::main(int argc, char** argv);
	};

	Application* create();

}

#endif // !_XENON_APPLICATION_H_
