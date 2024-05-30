#include "application.hpp"
#include "appData.hpp"
#include "devTools/logger/logger_core.hpp"
#include "input/input.hpp"
#include <functional>

namespace Core {

Application::Application() {
	XN_LOG_TO_FILE("Xenon-log");
	Core::AppData::init([this](auto && a) { pushEvent(std::forward<decltype(a)>(a)); });
	Input::init(Core::AppData::getWindow().passPointer());
	XN_LOG_TRC("Application: created");
}

Application::~Application() {
	try {
		Core::AppData::terminate();
		XN_LOG_TRC("Application: destroyed");
	} catch (std::exception& e) {
		try {
			std::cout << "something went terribly fucking wrong: " << e.what();
		} catch (std::exception& e2) { /* Holy fuck that is even worse lol */ }
	}
}

int Application::run() {
	try {
		while (m_running) {
			update();
			render();
			handleEvents();
			Input::resetStickyKeys();
			Core::AppData::getWindow().FEP();
		}
	} catch (std::exception& e) {
		try {
			XN_LOG_ERR("Exception in main application loop: {0}", e.what());
			return 1;
		} catch (std::exception&) {
			return 2;
		}
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
				Input::proccesEvents(Input::Action::Press, static_cast<int>(e.getArg().argUllong));
				break;
			case Core::Event::Type::KEY_RELESED:
				Input::proccesEvents(Input::Action::Relese, static_cast<int>(e.getArg().argUllong));
				break;
			case Core::Event::Type::MOUSE_MOVED:
				Input::proccesEvents(e.getArg().argFloat.first, e.getArg().argFloat.second);
				break;
			default:
				XN_LOG_ERR("Unknown event: " + e.getName());
		}
	}
}

void Application::pushEvent(const Core::Event& event) {
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
