#ifndef _XENON_CORE_EVENT_
#define _XENON_CORE_EVENT_

#include "api.h"
#include <cstdint>
#include <string>

namespace Xenon
{

	class XAPI Event 
	{
	public:
		enum class Type {
			KEY_RELESED,
			KEY_PRESSED,
			EMPTY_EVENT,
			WINDOW_CLOSE,
			WINDOW_RESIZE
		};
		Event(Type type = Event::Type::EMPTY_EVENT);
		Event(Type type, uint32_t arg0, uint32_t arg1);
		Event(Type type, int32_t arg0, int32_t arg1);
		Event(Type type, float arg0, float arg1);
		Event(Type type, uint64_t arg);

		union Argument {
			uint64_t ullong;
			struct { uint32_t uint0, uint1; }; //NOLINT
			struct { float float0, float1; }; //NOLINT
			Argument(uint64_t arg) : ullong(arg) {}
			Argument(uint32_t arg0, uint32_t arg1) : uint0(arg0), uint1(arg1) {}
			Argument(float arg0, float arg1) : float0(arg0), float1(arg1) {}
		};

		[[nodiscard]] Type getType() const;
		[[nodiscard]] std::string getName() const;
		[[nodiscard]] Argument getArg() const;
		

	private:
		Argument m_arg;
		Type m_type;
		

	};

}

#endif // !_XENON_CORE_EVENT_
