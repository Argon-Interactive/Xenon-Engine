#include "application.hpp"
#include "appData.hpp"
#include "devTools/logger/logger_core.hpp"
#include "input/input.hpp"
#include <functional>

namespace Xenon
{

	Application::Application() {
		XN_LOG_TO_FILE("Xenon-log");
		Core::AppData::init([this](auto && a) { pushEvent(std::forward<decltype(a)>(a)); });
		Xenon::Input::init(Core::AppData::getWindow().passPointer());
		XN_LOG_TRC("Application: created");
	}

	Application::~Application() {
		Core::AppData::terminate();
		XN_LOG_TRC("Application: destroyed");
	}

	int Application::run() {
		try {
			while (m_running) {
				update();
				render();
				handleEvents();
				Xenon::Input::resetStickyKeys();
				Core::AppData::getWindow().FEP();
			}
		} catch (std::exception& e) {
			XN_LOG_ERR("Exception in main application loop: {0}", e.what());
			return 1;
		}
		return 0;
	}

	void Application::render() {

	}

	void Application::update() {

	}

	void Application::handleEvents() {
		while(!emptyEventQueue()) {
			const Core::Event e = popEvent();
			switch (e.getType()) {
				case Core::Event::Type::WINDOW_CLOSE:
					XN_LOG_INF("Window close");
					m_running = false;
					break;
				case Core::Event::Type::WINDOW_RESIZE:
					XN_LOG_INF("Window resize: (width = {0}, height = {0})", e.getArg().argInt.first, e.getArg().argInt.second);
					break;
				case Core::Event::Type::KEY_PRESSED:
					Xenon::Input::proccesEvents(Xenon::Input::Action::Press, static_cast<int>(e.getArg().argUllong));
					break;
				case Core::Event::Type::KEY_RELESED:
					Xenon::Input::proccesEvents(Xenon::Input::Action::Relese, static_cast<int>(e.getArg().argUllong));
					break;
				case Core::Event::Type::MOUSE_MOVED:
					Xenon::Input::proccesEvents(e.getArg().argFloat.first, e.getArg().argFloat.second);
					break;
				default:
					XN_LOG_ERR("Unknown event: " + e.getName());
			}
		}
	}

	void Application::pushEvent(const Core::Event& event) noexcept {
		const std::lock_guard<std::mutex> lock(m_mutex);
		m_eventQueue.push(event);
		m_cond.notify_one();
	}

	Core::Event Application::popEvent() noexcept {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this] { return !m_eventQueue.empty(); } );
		Core::Event e = m_eventQueue.front();
		m_eventQueue.pop();
		return e;
	}

	bool Application::emptyEventQueue() const noexcept {
        const std::lock_guard<std::mutex> lock(m_mutex);
        return m_eventQueue.empty();
    }

}
