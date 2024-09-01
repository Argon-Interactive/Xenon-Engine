#pragma once
//TEST:

#include "ECS/reference.hpp"

namespace Core {

struct FComp : public Component {
	float val;
};

struct Comp : public Component {
	Comp(int i, FComp* p) : val(i), ref(p) {}
	int val;
	Reference<FComp> ref;
	char bloat[500] = {};
};

}
