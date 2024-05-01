#ifndef _XENON_APPLICATION_H_
#define _XENON_APPLICATION_H_

#include "../api.h"
#include "event/event.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>

extern int main(int argc, char** argv);

namespace Xenon
{

	class XAPI Application
	{
	public:
		Application();
		virtual ~Application();

		void pushEvent(const Event& event);

	private:
		int run();
		void handleEvents();
		void render();
		void update();

		bool m_running = true;

		Event popEvent();

		std::queue<Event> m_queue;
		mutable std::mutex m_mutex;
		std::condition_variable m_cond;

		friend int ::main(int argc, char** argv);
	};

	Application* create();

}

#endif // !_XENON_APPLICATION_H_
