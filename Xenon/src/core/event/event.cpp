#include "event.hpp"

namespace Xenon
{

	Event::Event(Event::Type type)
		:m_arg(0), m_type(type) {}

	Event::Event(Event::Type type, uint64_t arg)
		:m_arg(arg), m_type(type) {}

	Event::Event(Event::Type type, uint32_t arg0, uint32_t arg1)
	:m_arg({arg0, arg1}), m_type(type) {}

	Event::Event(Event::Type type, int32_t arg0, int32_t arg1)
	:m_arg({static_cast<uint32_t>(arg0), static_cast<uint32_t>(arg1)}), m_type(type) {}

	Event::Event(Event::Type type, float arg0, float arg1)
		:m_arg({arg0, arg1}), m_type(type) {}

	Event::Type Event::getType() const { return m_type; }

	Event::Argument Event::getArg() const { return m_arg; }

	std::string Event::getName() const {
		switch (m_type) {
			case Type::WINDOW_CLOSE:
				return "Window close event";
			default:
				return "Unknown event";
		}
	}

}
