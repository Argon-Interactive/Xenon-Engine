#ifndef _XENON_SYSTEM_COMPONENTS_TRANSFORM_
#define _XENON_SYSTEM_COMPONENTS_TRANSFORM_ 

#include "ECS/component.hpp"

namespace Core {

COMPONENT(Transform) {
	double x = 0;
	double y = 0;
	double angle = 0;
	double xScale = 1;
	double yScale = 1;
};

}

#endif // !_XENON_SYSTEM_COMPONENTS_TRANSFORM_
