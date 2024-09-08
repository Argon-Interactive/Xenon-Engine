#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <memory_resource>

#include "ECS/entity.hpp"
#include "ECS/componentCluster.hpp"
#include "System/Components/TestComp.hpp"
#include "System/Components/Transform.hpp"
#include "memory/debugMemoryResource.hpp"
#include "memory/chunkedResource.hpp"

int bench(Core::ComponentCluster& cc) {
	for(Core::Entity e = 1; e < 500; e++) {
		cc.get<Core::Comp>().addComponent(e);
		cc.get<Core::Transform>().addComponent(e);
		cc.get<Core::FComp>().addComponent(e);
	}
	for(Core::Entity e = 50; e < 100; e++) {
		cc.get<Core::Comp>().removeComponent(e);
		cc.get<Core::Transform>().removeComponent(e);
		cc.get<Core::FComp>().removeComponent(e);
	}
	for(Core::Entity e = 500; e < 2000; e++) {
		cc.get<Core::Comp>().addComponent(e);
		cc.get<Core::Transform>().addComponent(e);
		cc.get<Core::FComp>().addComponent(e);
	}
	for(Core::Entity e = 1800; e > 1500; e--) {
		cc.get<Core::Comp>().removeComponent(e);
		cc.get<Core::Transform>().removeComponent(e);
		cc.get<Core::FComp>().removeComponent(e);
	}
	for(Core::Entity e = 1; e < 50; e++) {
		cc.get<Core::Comp>().removeComponent(e);
		cc.get<Core::Transform>().removeComponent(e);
		cc.get<Core::FComp>().removeComponent(e);
	}
	for(Core::Entity e = 2000; e < 10000; e++) {
		cc.get<Core::Comp>().addComponent(e);
		cc.get<Core::Transform>().addComponent(e);
		cc.get<Core::FComp>().addComponent(e);
	}
	for(Core::Entity e = 4000; e < 10000; e++) {
		cc.get<Core::Comp>().removeComponent(e);
		cc.get<Core::Transform>().removeComponent(e);
		cc.get<Core::FComp>().removeComponent(e);
	}
	for(Core::Entity e = 5000; e < 12000; e++) {
		cc.get<Core::Comp>().addComponent(e);
		cc.get<Core::Transform>().addComponent(e);
		cc.get<Core::FComp>().addComponent(e);
	}
	return 5;
}

TEST_CASE("CR") {
	Core::DebugMemoryResource deb1("[Upstream]");
	Core::ChunkedResource cr(&deb1);
	// Core::DebugMemoryResource deb0("[Main]", &cr);
	Core::ComponentCluster cc(&cr);
	for(Core::Entity e = 1; e < 10000; e++) {
		cc.get<Core::Comp>().addComponent(e);
		cc.get<Core::Transform>().addComponent(e);
		cc.get<Core::FComp>().addComponent(e);
	}
	REQUIRE(true);
}

TEST_CASE("ComponentCluster with ChunkedResource performance", "[ECS] [ComponentCluster] [ChunkedResource]") {
	BENCHMARK("New/Delete resource") {
		Core::ComponentCluster cc(std::pmr::new_delete_resource());
		return bench(cc);
	};
	BENCHMARK("ChunkedResource resource") {
		// Core::DebugMemoryResource deb1("[Upstream]");
		Core::ChunkedResource cr;
		// Core::DebugMemoryResource deb0("[Main]", &cr);
		Core::ComponentCluster cc(&cr);
		return bench(cc);
	};
}
