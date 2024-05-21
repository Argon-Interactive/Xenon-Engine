#include "event.hpp"

namespace Xenon
{

	Event::Event(Event::Type type)
		:m_type(type) {}

	Event::Type Event::getType() const { return m_type; }

	int Event::getValue() const { return m_value; }

	std::string Event::getName() const {
		switch (m_type) {
			case Type::WINDOW_CLOSE:
				return "Window close event";
			default:
				return "Unknown event";
		}
	}

}
