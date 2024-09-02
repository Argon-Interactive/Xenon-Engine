#pragma once
//TEST:

#include "ECS/component.hpp"
#include "ECS/reference.hpp"

namespace Core {

COMPONENT(FComp) {
	float val;
};

COMPONENT(Comp) {
	int val;
	Reference<FComp> ref;
	char bloat[500] = {};
};

}
