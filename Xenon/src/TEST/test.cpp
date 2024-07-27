#include "test.hpp"

#include "ECS/componentList.hpp"
#include "devTools/logger_core.hpp"

void Core::Test::test() {
	Core::ComponentPool<int> cp1;
	Core::ComponentPool<float> cp2;
	Core::ComponentPool<char> cp3;
	//
	Core::ComponentPool<int> cp12;
	Core::ComponentPool<float> cp22;
	Core::ComponentPool<char> cp32;
	//
	Core::ComponentPool<int> cp13;
	Core::ComponentPool<float> cp23;
	Core::ComponentPool<char> cp33;

	Core::Entity ent = 0;

	cp1.addEntity(ent++, 1);
	cp1.addEntity(ent++, 2);

	cp2.addEntity(ent++, 3.14f);
	cp2.addEntity(ent++, 6.28f);
	cp2.addEntity(ent++, 9.42f);

	cp3.addEntity(ent++, 'a');
	cp3.addEntity(ent++, 'b');

	cp12.addEntity(ent++, 3);
	cp12.addEntity(ent++, 4);

	cp22.addEntity(ent++, 12.56f);
	cp22.addEntity(ent++, 15.7f);
	cp22.addEntity(ent++, 18.84f);

	cp32.addEntity(ent++, 'c');
	cp32.addEntity(ent++, 'd');

	cp13.addEntity(ent++, 5);
	cp13.addEntity(ent++, 6);

	cp23.addEntity(ent++, 21.98f);
	cp23.addEntity(ent++, 25.12f);
	cp23.addEntity(ent++, 28.26f);

	cp33.addEntity(ent++, 'e');
	cp33.addEntity(ent++, 'f');

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

	XN_LOG_ENT("Int size: {0}", intList.size());
	XN_LOG_ENT("Int pool 1 size: {0}", cp1.data().size());
	cp1.data().loginx();

	XN_LOG_ENT("Printing int pool 1");
	for(const auto a : cp1.data()) {
		XN_LOG_ENT(a);
	}
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

}
