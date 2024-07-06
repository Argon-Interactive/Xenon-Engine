#ifndef _XENON_CORE_APPLICATION_
#define _XENON_CORE_APPLICATION_

#include "event.hpp"
#include "Xenon/scene.hpp"

#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>

namespace Xenon { class Application; }

namespace Core {

class Application {
public:
	Application(std::function<void(void*)> configFunction, std::vector<std::function<void(Xenon::Scene*)>> buildFunctions);
	~Application();
	Application(const Application&) = delete;
	Application(Application&&) noexcept = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	void pushEvent(const Core::Event& event);

private:
	int run();
	void handleEvents();
	void update(double deltaT);
	void fixedUpdate();
	void render();

	bool m_running = true;

	Core::Event popEvent() noexcept;
	bool emptyEventQueue() const noexcept;

	std::queue<Core::Event> m_eventQueue;
	mutable std::mutex m_mutex;
	std::condition_variable m_cond;

	friend class Xenon::Application;
};

}

#endif // !_XENON_CORE_APPLICATION_
