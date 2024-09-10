#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <random>

#include "ECS/componentCluster.hpp"
#include "ECS/entity.hpp"
#include "System/Components/TestComp.hpp"
#include "System/Components/Transform.hpp"

TEST_CASE( "ComponentCluster", "[ECS] [Component] [ComponentCluster]" ) {
	std::random_device rd;
	std::mt19937 gen(rd());

	auto _ = GENERATE(range(1, 20));
	Core::ComponentCluster cc;
	std::unordered_map<Core::Entity, unsigned int> entMap;
	for(Core::Entity e = 1; e < 101; e++)
		entMap[e] = 0;
	SECTION("Adding independent components") {
		std::uniform_int_distribution<> dis(1, 7);
		for(Core::Entity entity = 1; entity < 101; entity++) {
			auto i = static_cast<unsigned int>(dis(gen));
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

			for(Core::Entity e = 1; e < 101; e++) {
				auto t = entMap[e];
				if((t & 1u) != 0u) REQUIRE(cc.get<Core::Comp>().getComponent(e).val == static_cast<int>(e) + 1000);
				if((t & 2u) != 0u) REQUIRE(cc.get<Core::FComp>().getComponent(e).val == static_cast<float>(e) + 0.23f);
				if((t & 4u) != 0u) REQUIRE(cc.get<Core::Transform>().getComponent(e).x == static_cast<double>(e) + 0.42);
			}
		}
	}
	SECTION("Removing components") {
		std::uniform_int_distribution<> dis(1, 7);
		for(Core::Entity entity = 1; entity < 101; entity++) {
			auto i = static_cast<unsigned int>(dis(gen));
			auto j = entMap[entity];
			entMap[entity] = j & (~i);
			if((i & j & 1u) != 0u) cc.get<Core::Comp>().removeComponent(entity);
			if((i & j & 2u) != 0u) cc.get<Core::FComp>().removeComponent(entity);
			if((i & j & 4u) != 0u) cc.get<Core::Transform>().removeComponent(entity);

			for(Core::Entity e = 1; e < 101; e++) {
				auto t = entMap[e];
				if((t & 1u) != 0u) REQUIRE(cc.get<Core::Comp>().getComponent(e).val == static_cast<int>(e) + 1000);
				if((t & 2u) != 0u) REQUIRE(cc.get<Core::FComp>().getComponent(e).val == static_cast<float>(e) + 0.23f);
				if((t & 4u) != 0u) REQUIRE(cc.get<Core::Transform>().getComponent(e).x == static_cast<double>(e) + 0.42);
			}
		}
	}
	SECTION("Adding and removing components") {
		std::uniform_int_distribution<> dis(1, 15);
		for(Core::Entity entity = 1; entity < 101; entity++) {
			auto i = static_cast<unsigned int>(dis(gen));
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

			for(Core::Entity e = 1; e < 101; e++) {
				auto t = entMap[e];
				if((t & 1u) != 0u) REQUIRE(cc.get<Core::Comp>().getComponent(e).val == static_cast<int>(e) + 1000);
				if((t & 2u) != 0u) REQUIRE(cc.get<Core::FComp>().getComponent(e).val == static_cast<float>(e) + 0.23f);
				if((t & 4u) != 0u) REQUIRE(cc.get<Core::Transform>().getComponent(e).x == static_cast<double>(e) + 0.42);
			}
		}
	}
}

TEST_CASE( "ComponentDependency", "[ECS] [Component] [Reference] [ComponentCluster]" ) {
	std::random_device rd;
	std::mt19937 gen(rd());

	auto _ = GENERATE(range(1, 50));
	Core::ComponentCluster cc;
	std::unordered_map<Core::Entity, Core::Entity> depMap;
	for(Core::Entity e = 1; e < 101; e++) {
		depMap[e] = 0;
		cc.get<Core::FComp>().addComponent(e);
		cc.get<Core::FComp>().getComponent(e).val = static_cast<float>(e) + 0.23f;
	}
	SECTION("Adding components with dependency") {
		std::uniform_int_distribution<> dis(1, 150);
		for(Core::Entity entity = 1; entity < 101; entity++) {
			auto i = static_cast<unsigned int>(dis(gen));
			if(i <= 100) {
				depMap[entity] = i;
				cc.get<Core::Comp>().addComponent(entity);
				cc.get<Core::Comp>().getComponent(entity).ref.set(cc.get<Core::FComp>().getComponent(i));
			} 

			for(Core::Entity e = 1; e < 101; e++) {
				auto t = depMap[e];
				if(t != 0) REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == cc.get<Core::FComp>().getComponentPtr(t));
			}
		}
	}
	SECTION("Removing components with dependency") {
		std::uniform_int_distribution<> dis(1, 200);
		for(Core::Entity entity = 1; entity < 101; entity++) {
			auto i = static_cast<unsigned int>(dis(gen));
			if(i <= 100 && depMap[entity] != 0) {
				depMap[entity] = 0;
				cc.get<Core::Comp>().removeComponent(entity);
			} 

			for(Core::Entity e = 1; e < 101; e++) {
				auto t = depMap[e];
				if(t != 0) REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == cc.get<Core::FComp>().getComponentPtr(t));
			}
		}
	}
	SECTION("Removing dependent components") {
		std::uniform_int_distribution<> dis(1, 4);
		for(Core::Entity entity = 1; entity < 101; entity++) {
			auto i = static_cast<unsigned int>(dis(gen));
			if(i == 0) cc.get<Core::FComp>().removeComponent(entity);

			for(Core::Entity e = 1; e < 101; e++) {
				auto t = depMap[e];
				if(t != 0) {
					if(cc.get<Core::FComp>().hasComponent(t))
						REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == cc.get<Core::FComp>().getComponentPtr(t));
					else
						REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == nullptr);
				}
			}
		}
	}
	SECTION("Changing dependency") {
		std::uniform_int_distribution<> dis(1, 200);
		for(Core::Entity entity = 1; entity < 101; entity++) {
			auto i = static_cast<unsigned int>(dis(gen));
			if(i <= 100 && cc.get<Core::Comp>().hasComponent(entity) && cc.get<Core::FComp>().hasComponent(i)) {
				depMap[entity] = i;
				cc.get<Core::Comp>().getComponent(entity).ref.set(cc.get<Core::FComp>().getComponent(i));
			}

			for(Core::Entity e = 1; e < 101; e++) {
				auto t = depMap[e];
				if(t != 0) {
					if(cc.get<Core::FComp>().hasComponent(t))
						REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == cc.get<Core::FComp>().getComponentPtr(t));
					else
						REQUIRE(cc.get<Core::Comp>().getComponent(e).ref.getPtr() == nullptr);
				}
			}
		}
	}
}
