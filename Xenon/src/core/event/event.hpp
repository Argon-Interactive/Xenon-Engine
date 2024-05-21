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
			WINDOW_RESIZE
		};

		Event(Type type = Event::Type::EMPTY_EVENT);
		Event(Type type, int x, int y);

		Type getType();
		std::string getName();

		struct {
			int x;
			int y;
		} data;

	private:
		Type m_type;
	};

}

#endif // !_XENON_CORE_EVENT_
