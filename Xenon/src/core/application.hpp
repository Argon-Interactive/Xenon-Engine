#ifndef _XENON_APPLICATION_
#define _XENON_APPLICATION_

#include "api.h"
#include "core/input/input.hpp"
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
		Application(const Application&); //TODO implement or delete
		Application(Application&&) noexcept; //TODO implement or delete
		Application& operator=(const Application&); //TODO implement or delete
		Application& operator=(Application&&) noexcept; //TODO implement or delete

		void pushEvent(const Core::Event& event);

	private:
		int run();
		void handleEvents();
		void render();
		void update();

		bool m_running = true;

		Core::Event popEvent();
		bool emptyEventQueue() const;

		std::queue<Core::Event> m_eventQueue;
		mutable std::mutex m_mutex;
		std::condition_variable m_cond;

		friend int ::main(int argc, char** argv);
	};

	Application* create();

}

#endif // !_XENON_APPLICATION_
