#include "application.hpp"
#include "appData.hpp"
#include "input/input.hpp"

#include "devTools/logger_core.hpp"

// TEST:
#include "System/parenting.hpp"
#include "System/Components/Transform.hpp"

#include <exception>

namespace Core {

Application::Application(const Xenon::AppConfig& config) {
	XN_LOG_TO_FILE("Xenon-log");
	Core::AppData::init([this](auto && a) { pushEvent(std::forward<decltype(a)>(a)); }, config);
	XN_LOG_TRC("Application: created");
}

Application::~Application() {
	try {
		Core::AppData::terminate();
		XN_LOG_TRC("Application: destroyed");
	} catch (std::exception& e) {
		try {
			std::cout << "something went terribly fucking wrong: " << e.what();
		} catch (std::exception& e2) { 
			exit(-1); // This is never gonna happen basically
		}
	}
}

int Application::run() {
	try {
		while (m_running) {
			update(1);
			fixedUpdate();
			render();
			handleEvents();
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

void Application::update([[maybe_unused]] double deltaT) {
	// TEMP: there should propably be a better place to do this
	updateChildPositions();
}

void Application::fixedUpdate() {
	static Entity e0 = 0;
	static Entity e1 = 0;
	auto* scene = AppData::getSceneManager().getActiveScene();

	if(Input::getKeyPress(XN_KEY_1)) {
		e0 = scene->createEntity();
	}
	if(Input::getKeyPress(XN_KEY_2)) {
		e1 = scene->createChild(e0);
	}
	if(Input::getKeyPress(XN_KEY_3)) {
		scene->getComponent<Transform>(e0).x += 1;
	}
	if(Input::getKeyPress(XN_KEY_4)) {
		scene->deleteEntity(e1);
	}
	if(Input::getKeyPress(XN_KEY_5)) {
		scene->deleteEntity(e0);
	}
}

void Application::handleEvents() {
	const std::lock_guard<std::mutex> lock(m_mutex);
	while(!m_eventQueue.empty()) {
		const Core::Event e = m_eventQueue.front();
		m_eventQueue.pop();
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
	Input::resetStickyKeys();
}

void Application::pushEvent(const Core::Event& event) {
	const std::lock_guard<std::mutex> lock(m_mutex);
	m_eventQueue.push(event);
}

}
