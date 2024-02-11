#ifndef _XENON_APPLICATION_H_
#define _XENON_APPLICATION_H_


#include "core.h"

namespace Xenon
{

	class XAPI Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	Application* create();

}





#endif // !_XENON_APPLICATION_H_
