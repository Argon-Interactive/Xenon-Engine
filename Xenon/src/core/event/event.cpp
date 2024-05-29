#include "event.hpp"

namespace Core
{

	Event::Event(Event::Type type) noexcept
		:m_arg(0), m_type(type) {}

	Event::Event(Event::Type type, uint64_t arg) noexcept
		:m_arg(arg), m_type(type) {}

	Event::Event(Event::Type type, uint32_t arg0, uint32_t arg1) noexcept
	:m_arg({arg0, arg1}), m_type(type) {}

	Event::Event(Event::Type type, int32_t arg0, int32_t arg1) noexcept
	:m_arg({static_cast<uint32_t>(arg0), static_cast<uint32_t>(arg1)}), m_type(type) {}

	Event::Event(Event::Type type, float arg0, float arg1) noexcept
		:m_arg({arg0, arg1}), m_type(type) {}

	Event::Type Event::getType() const noexcept {
		return m_type; 
	}

	Event::Argument Event::getArg() const noexcept { 
		return m_arg; 
	}

	std::string Event::getName() const {
		switch (m_type) {
			case Type::WINDOW_CLOSE:
				return "Window close event";
			case Type::WINDOW_RESIZE:
				return "Window resize event";	
			case Type::KEY_PRESSED:
				return "Key pressed event";
			case Type::KEY_RELESED:
				return "Key relesed event";
			case Type::MOUSE_MOVED:
				return "Mouse new position event";
			default:
				return "Unknown event";
		}
	}

}
