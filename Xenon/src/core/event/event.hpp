#ifndef _XENON_CORE_EVENT_
#define _XENON_CORE_EVENT_

#include "api.h"
#include <string>

namespace Xenon
{

	class XAPI Event 
	{
	public:
		enum class Type {
			EMPTY_EVENT,
			WINDOW_CLOSE,
			WINDOW_RESIZE,
			KEY_PRESSED,
			KEY_RELESED
		};
		Event(Type type = Event::Type::EMPTY_EVENT);

		Type getType() const;
		std::string getName() const;

	private:
		Type m_type;
	};

}

#endif // !_XENON_CORE_EVENT_
