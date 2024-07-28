#include "test.hpp"

#include "ECS/componentList.hpp"
#include "devTools/logger_core.hpp"

void Core::Test::test() {
	Core::ChunkedArray<int> cp1;
	Core::ChunkedArray<float> cp2;
	Core::ChunkedArray<char> cp3;
	//
	Core::ChunkedArray<int> cp12;
	Core::ChunkedArray<float> cp22;
	Core::ChunkedArray<char> cp32;
	//
	Core::ChunkedArray<int> cp13;
	Core::ChunkedArray<float> cp23;
	Core::ChunkedArray<char> cp33;

	cp1.emplace_back(1);
	cp1.emplace_back(2);

	cp2.emplace_back(3.14f);
	cp2.emplace_back(6.28f);
	cp2.emplace_back(9.42f);

	cp3.emplace_back('a');
	cp3.emplace_back('b');

	cp12.emplace_back(3);
	cp12.emplace_back(4);

	cp22.emplace_back(12.56f);
	cp22.emplace_back(15.7f);
	cp22.emplace_back(18.84f);

	cp32.emplace_back('c');
	cp32.emplace_back('d');

	cp13.emplace_back(5);
	cp13.emplace_back(6);

	cp23.emplace_back(21.98f);
	cp23.emplace_back(25.12f);
	cp23.emplace_back(28.26f);

	cp33.emplace_back('e');
	cp33.emplace_back('f');

	Core::ComponentReferenceList<int> intList;
	intList.push(&cp1);
	intList.push(&cp12);
	intList.push(&cp13);
	Core::ComponentReferenceList<float> floatList;
	floatList.push(&cp2);
	floatList.push(&cp22);
	floatList.push(&cp23);
	Core::ComponentReferenceList<char> charList;
	charList.push(&cp3);
	charList.push(&cp32);
	charList.push(&cp33);

	XN_LOG_ENT("ints: 1, 2, 3, 4, 5, 6");
	for(const auto a : intList) {
		XN_LOG_ENT(a);
	}
	XN_LOG_ENT("floats: 3.14, 6.28, 9.42, 12.56, 15.7, 18.84, 21.98, 25.12, 28.26");
	for(const auto a : floatList) {
		XN_LOG_ENT(a);
	}
	XN_LOG_ENT("chars: a, b, c, d, e, f");
	for(const auto a : charList) {
		XN_LOG_ENT(a);
	}
	XN_LOG_BR();
	intList.pop(&cp1);
	XN_LOG_ENT("ints: 3, 4, 5, 6");
	for(const auto a : intList) {
		XN_LOG_ENT(a);
	}
	floatList.pop(&cp22);
	XN_LOG_ENT("floats: 3.14, 6.28, 9.42, 21.98, 25.12, 28.26");
	for(const auto a : floatList) {
		XN_LOG_ENT(a);
	}
	charList.pop(&cp33);
	XN_LOG_ENT("chars: a, b, c, d");
	for(const auto a : charList) {
		XN_LOG_ENT(a);
	}
	

}
