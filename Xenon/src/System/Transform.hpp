#ifndef _XENON_SYSTEM_TRANSFORM_
#define _XENON_SYSTEM_TRANSFORM_

#include "ECS/component.hpp"
namespace Core {

COMPONENT(Transform) {
public:
	double x{}, y{}, z{};
};

}

#endif // !_XENON_SYSTEM_TRANSFORM_
