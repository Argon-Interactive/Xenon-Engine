#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include "ECS/componentCluster.hpp"
#include "ECS/entity.hpp"
#include "System/Components/TestComp.hpp"
#include "System/Components/Transform.hpp"

TEST_CASE( "Check ComponentCluster", "[ECS][Component][ComponentCluster]" ) {
	auto _ = GENERATE(range(1, 20));
	Core::ComponentCluster cc;
	std::unordered_map<Core::Entity, unsigned int> entMap;
	for(Core::Entity e = 1; e < 101; e++)
		entMap[e] = 0;
	SECTION("Adding independent components") {
		Core::Entity entity = GENERATE(range(1, 100));
		unsigned int i = GENERATE(take(1, random(1, 7)));
		entMap[entity] = i;
		if((i & 1u) != 0u) {
			cc.get<Core::Comp>().addComponent(entity);
			cc.get<Core::Comp>().getComponent(entity).val = static_cast<int>(entity) + 1000;
		}
		if((i & 2u) != 0u) {
			cc.get<Core::FComp>().addComponent(entity);
			cc.get<Core::FComp>().getComponent(entity).val = static_cast<float>(entity) + 0.23f;
		}
		if((i & 4u) != 0u) {
			cc.get<Core::Transform>().addComponent(entity);
			cc.get<Core::Transform>().getComponent(entity).x = static_cast<double>(entity) + 0.42;
		}
		SECTION("Test components integrity"){
			Core::Entity e = GENERATE(range(1, 100));
			auto t = entMap[e];
			if((t & 1u) != 0u) REQUIRE(cc.get<Core::Comp>().getComponent(e).val == static_cast<int>(e) + 1000);
			if((t & 2u) != 0u) REQUIRE(cc.get<Core::FComp>().getComponent(e).val == static_cast<float>(e) + 0.23f);
			if((t & 4u) != 0u) REQUIRE(cc.get<Core::Transform>().getComponent(e).x == static_cast<double>(e) + 0.42);
		}
	}
	SECTION("Removing components") {
		Core::Entity entity = GENERATE(range(1, 100));
		unsigned int i = GENERATE(take(1, random(1, 7)));
		auto j = entMap[entity];
		entMap[entity] = j & (~i);
		if((i & j & 1u) != 0u) cc.get<Core::Comp>().removeComponent(entity);
		if((i & j & 2u) != 0u) cc.get<Core::FComp>().removeComponent(entity);
		if((i & j & 4u) != 0u) cc.get<Core::Transform>().removeComponent(entity);
		SECTION("Test components integrity"){
			Core::Entity e = GENERATE(range(1, 100));
			auto t = entMap[e];
			if((t & 1u) != 0u) REQUIRE(cc.get<Core::Comp>().getComponent(e).val == static_cast<int>(e) + 1000);
			if((t & 2u) != 0u) REQUIRE(cc.get<Core::FComp>().getComponent(e).val == static_cast<float>(e) + 0.23f);
			if((t & 4u) != 0u) REQUIRE(cc.get<Core::Transform>().getComponent(e).x == static_cast<double>(e) + 0.42);
		}
	}
	SECTION("Adding and removing components") {
		Core::Entity entity = GENERATE(take(100, random(1, 100)));
		unsigned int i = GENERATE(take(1, random(1, 15)));
		auto j = entMap[entity];
		if((i & 8u) != 0u) {
			entMap[entity] = j & (~i);
			if((i & j & 1u) != 0u) cc.get<Core::Comp>().removeComponent(entity);
			if((i & j & 2u) != 0u) cc.get<Core::FComp>().removeComponent(entity);
			if((i & j & 4u) != 0u) cc.get<Core::Transform>().removeComponent(entity);
		} else {
			entMap[entity] = j | i;
			if((i & 1u) != 0u) {
				cc.get<Core::Comp>().addComponent(entity);
				cc.get<Core::Comp>().getComponent(entity).val = static_cast<int>(entity) + 1000;
			}
			if((i & 2u) != 0u) {
				cc.get<Core::FComp>().addComponent(entity);
				cc.get<Core::FComp>().getComponent(entity).val = static_cast<float>(entity) + 0.23f;
			}
			if((i & 4u) != 0u) {
				cc.get<Core::Transform>().addComponent(entity);
				cc.get<Core::Transform>().getComponent(entity).x = static_cast<double>(entity) + 0.42;
			}
		}
		SECTION("Test components integrity"){
			Core::Entity e = GENERATE(range(1, 100));
			auto t = entMap[e];
			if((t & 1u) != 0u) REQUIRE(cc.get<Core::Comp>().getComponent(e).val == static_cast<int>(e) + 1000);
			if((t & 2u) != 0u) REQUIRE(cc.get<Core::FComp>().getComponent(e).val == static_cast<float>(e) + 0.23f);
			if((t & 4u) != 0u) REQUIRE(cc.get<Core::Transform>().getComponent(e).x == static_cast<double>(e) + 0.42);
		}
	}
}

TEST_CASE( "Check component dependency system", "[ECS][Component][ComponentCluster][ComponentDependency]" ) {
	auto _ = GENERATE(range(1, 50));
	Core::ComponentCluster cc;
	std::unordered_map<Core::Entity, Core::Entity> depMap;
	for(Core::Entity e = 1; e < 101; e++) {
		depMap[e] = 0;
		cc.get<Core::FComp>().addComponent(e);
		cc.get<Core::FComp>().getComponent(e).val = static_cast<float>(e) + 0.23f;
	}
	SECTION("Adding components with dependency") {
		Core::Entity entity = GENERATE(range(1, 100));
		unsigned int i = GENERATE(take(1, random(1, 150)));
		if(i <= 100) {
			depMap[entity] = i;
			cc.get<Core::Comp>().addComponent(entity);
			cc.get<Core::Comp>().getComponent(entity).ref.set(cc.get<Core::FComp>().getComponent(i));
		} 
		SECTION("Test dependency integrity"){
			Core::Entity e = GENERATE(range(1, 100));
			auto t = depMap[e];
			if(t != 0) REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == cc.get<Core::FComp>().getComponentPtr(t));
		}
	}
	SECTION("Removing components with dependency") {
		Core::Entity entity = GENERATE(range(1, 100));
		unsigned int i = GENERATE(take(1, random(1, 200)));
		if(i <= 100 && depMap[entity] != 0) {
			depMap[entity] = 0;
			cc.get<Core::Comp>().removeComponent(entity);
		} 
		SECTION("Test dependency integrity"){
			Core::Entity e = GENERATE(range(1, 100));
			auto t = depMap[e];
			if(t != 0) REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == cc.get<Core::FComp>().getComponentPtr(t));
		}
	}
	SECTION("Removing dependent components") {
		Core::Entity entity = GENERATE(range(1, 100));
		unsigned int i = GENERATE(take(1, random(0, 4)));
		if(i == 0) cc.get<Core::FComp>().removeComponent(entity);

		SECTION("Test dependency integrity"){
			Core::Entity e = GENERATE(range(1, 100));
			auto t = depMap[e];
			if(t != 0) {
				if(cc.get<Core::FComp>().hasComponent(t)) {
					REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == cc.get<Core::FComp>().getComponentPtr(t));
				} else {
					REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == nullptr);
				}
			}
		}
	}
	SECTION("Changing dependency") {
		Core::Entity entity = GENERATE(range(1, 100));
		unsigned int i = GENERATE(take(1, random(1, 200)));
		if(i <= 100 && cc.get<Core::Comp>().hasComponent(entity) && cc.get<Core::FComp>().hasComponent(i)) {
			depMap[entity] = i;
			cc.get<Core::Comp>().getComponent(entity).ref.set(cc.get<Core::FComp>().getComponent(i));
		}

		SECTION("Test dependency integrity"){
			Core::Entity e = GENERATE(range(1, 100));
			auto t = depMap[e];
			if(t != 0) {
				if(cc.get<Core::FComp>().hasComponent(t)) {
					REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == cc.get<Core::FComp>().getComponentPtr(t));
				} else {
					REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == nullptr);
				}
			}
		}
	}
	SECTION("Everything together") {
		auto _ = GENERATE(range(1, 5));
		Core::Entity entity = GENERATE(range(1, 100));
		unsigned int i = GENERATE(take(1, random(1, 400)));
		if(i <= 100) {
			depMap[entity] = i;
			cc.get<Core::Comp>().addComponent(entity);
			cc.get<Core::Comp>().getComponent(entity).ref.set(cc.get<Core::FComp>().getComponent(i));
		} else if(i <= 100 && depMap[entity] != 0) {
			depMap[entity] = 0;
			cc.get<Core::Comp>().removeComponent(entity);
		} else if(i <= 200 && cc.get<Core::FComp>().hasComponent(entity)) {
			cc.get<Core::FComp>().removeComponent(entity);
		} else if(i <= 300 && depMap[entity] != 0 && cc.get<Core::FComp>().hasComponent(i - 200)) {
			depMap[entity] = i - 200;
			cc.get<Core::Comp>().getComponent(entity).ref.set(cc.get<Core::FComp>().getComponent(i - 200));
		}

		SECTION("Test dependency integrity"){
			Core::Entity e = GENERATE(range(1, 100));
			auto t = depMap[e];
			if(t != 0) {
				if(cc.get<Core::FComp>().hasComponent(t)) {
					REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == cc.get<Core::FComp>().getComponentPtr(t));
				} else {
					REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == nullptr);
				}
			}
		}
	}
}
