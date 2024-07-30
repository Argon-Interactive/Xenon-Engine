#pragma once
//TEST:
struct Comp {
	Comp(int i, float* p) : val(i), ref(p) {}
	int val;
	float* ref;
	char bloat[500] = {};
};
