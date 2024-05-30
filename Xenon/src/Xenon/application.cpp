#include "application.hpp"
#include "core/application.hpp"

namespace Xenon {

Application::Application()
	:m_impl(std::make_unique<Core::Application>()) {}

Application::~Application() = default;

int Application::run() {
	return m_impl->run();
}

}
