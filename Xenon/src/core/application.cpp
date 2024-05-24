#include "application.hpp"
#include "appData.hpp"
#include "devTools/logger/logger_core.hpp"
#include "input/input.hpp"
#include <X11/Xlib.h>
#include <functional>

namespace Xenon
{

	Application::Application() {
		XN_LOG_TO_FILE("Xenon-log");
		Core::AppData::init(std::bind(&Application::pushEvent, this, std::placeholders::_1));
		Xenon::Input::init();
		XN_LOG_TRC("Application: created");
	}

	Application::~Application() {
		Core::AppData::terminate();
		XN_LOG_TRC("Application: destroyed");
	}

	int Application::run() {
		while (m_running) {
			// aplikacja dziaua
			handleEvents();
			Xenon::Input::resetStickyKeys();
			Core::AppData::getWindow().FEP();
		}
		return 0;
	}

	void Application::handleEvents() {
		while(!emptyEventQueue()) {
			Event e = popEvent();
			switch (e.getType()) {
				case Event::Type::WINDOW_CLOSE:
					XN_LOG_INF("Window close");
					m_running = false;
					break;
				case Event::Type::WINDOW_RESIZE:
					XN_LOG_INF("Window resize: (width = {0}, height = {0})", e.getArg().uint0, e.getArg().uint1);
					break;
				case Event::Type::KEY_PRESSED:
					Xenon::Input::proccesEvents(Xenon::Input::Action::Press, static_cast<int>(e.getArg().ullong));
					break;
				case Event::Type::KEY_RELESED:
					Xenon::Input::proccesEvents(Xenon::Input::Action::Relese, static_cast<int>(e.getArg().ullong));
					break;
				default:
					XN_LOG_ERR("Unknown event: " + e.getName());
			}
		}
	}

	void Application::pushEvent(const Event& event) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_eventQueue.push(event);
		m_cond.notify_one();
	}

	Event Application::popEvent() {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this] { return !m_eventQueue.empty(); } );
		Event e = m_eventQueue.front();
		m_eventQueue.pop();
		return e;
	}

	bool Application::emptyEventQueue() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_eventQueue.empty();
    }

}
