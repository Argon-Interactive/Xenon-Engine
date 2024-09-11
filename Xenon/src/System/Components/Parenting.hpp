#ifndef _XENON_SYSTEM_COMPONENTS_PARENTING_
#define _XENON_SYSTEM_COMPONENTS_PARENTING_ 

#include "ECS/component.hpp"
#include "ECS/reference.hpp"
#include "Transform.hpp"

namespace Core {

struct Parent;

COMPONENT(Child) {
	double x = 0;
	double y = 0;
	double angle = 0;
	double xScale = 0;
	double yScale = 0;
	Reference<Parent> parent;
	Reference<Child> next;
	Reference<Child> prev;
	Reference<Transform> transform;
	Reference<Parent> selfParent;
};

COMPONENT(Parent) {
	bool root;
	Reference<Transform> transform;
	Reference<Child> childList;
};

}

#endif // !_XENON_SYSTEM_COMPONENTS_PARENTING_
