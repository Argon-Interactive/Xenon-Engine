#ifndef _XENON_CORE_EVENT_
#define _XENON_CORE_EVENT_

#include <string>

namespace Core
{


	class Event 
	{
	public:
		enum class Type {
			WINDOW_CLOSE
		};

		Event(Type type);

		Type getType();
		std::string getName();

	private:
		Type m_type;
	};


}

#endif // !_XENON_CORE_EVENT_
