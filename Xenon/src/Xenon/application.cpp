#include "application.hpp"
#include "core/application.hpp"

namespace Xenon {

Application::Application(const AppConfig& config)
	:m_impl(new Core::Application(config)) {}

Application::~Application() {
	delete m_impl;
}

int Application::run() {
	return m_impl->run();
}

}
