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
	for(Core::Entity e = 1; e < 5000; e++) {
		cc.get<Core::Comp>().addComponent(e);
		cc.get<Core::Transform>().addComponent(e);
		cc.get<Core::FComp>().addComponent(e);
	}
	for(Core::Entity e = 500; e < 1000; e++) {
		cc.get<Core::Comp>().removeComponent(e);
		cc.get<Core::Transform>().removeComponent(e);
		cc.get<Core::FComp>().removeComponent(e);
	}
	for(Core::Entity e = 5000; e < 20000; e++) {
		cc.get<Core::Comp>().addComponent(e);
		cc.get<Core::Transform>().addComponent(e);
		cc.get<Core::FComp>().addComponent(e);
	}
	for(Core::Entity e = 18000; e > 15000; e--) {
		cc.get<Core::Comp>().removeComponent(e);
		cc.get<Core::Transform>().removeComponent(e);
		cc.get<Core::FComp>().removeComponent(e);
	}
	for(Core::Entity e = 1; e < 500; e++) {
		cc.get<Core::Comp>().removeComponent(e);
		cc.get<Core::Transform>().removeComponent(e);
		cc.get<Core::FComp>().removeComponent(e);
	}
	for(Core::Entity e = 20000; e < 100000; e++) {
		cc.get<Core::Comp>().addComponent(e);
		cc.get<Core::Transform>().addComponent(e);
		cc.get<Core::FComp>().addComponent(e);
	}
	for(Core::Entity e = 40000; e < 100000; e++) {
		cc.get<Core::Comp>().removeComponent(e);
		cc.get<Core::Transform>().removeComponent(e);
		cc.get<Core::FComp>().removeComponent(e);
	}
	for(Core::Entity e = 50000; e < 1000000; e++) {
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
	REQUIRE(bench(cc) == 5);
}

TEST_CASE("ComponentCluster with ChunkedResource performance", "[ECS] [ComponentCluster] [ChunkedResource]") {
	BENCHMARK("New/Delete resource") {
		Core::ComponentCluster cc(std::pmr::new_delete_resource());
		return bench(cc);
	};
	BENCHMARK("ChunkedResource resource") {
		Core::ChunkedResource cr;
		Core::ComponentCluster cc(&cr);
		return bench(cc);
	};
}
