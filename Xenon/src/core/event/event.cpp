#include "event.hpp"

namespace Xenon
{

	Event::Event(Event::Type type)
		:m_type(type) {}

	Event::Type Event::getType() {
		return m_type;
	}

	std::string Event::getName() {
		switch (m_type) {
			case Type::WINDOW_CLOSE:
				return "Window close event";
			default:
				return "Unknown event";
		}
	}

}
