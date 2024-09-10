#include <catch2/catch_all.hpp>

#include "ECS/chunkedArray.hpp"

bool calledCon = false;
bool calledDes = false;
bool calledCop = false;
bool calledMov = false;
bool calledCAs = false;
bool calledMAs = false;

struct Test {
	Test() { calledCon = true; }
	~Test() { calledDes = true; }

	Test(const Test & /*unused*/) { calledCop = true; }
	Test(Test && /*unused*/) noexcept { calledMov = true; }

	Test &operator=(const Test& other) { 
		if(this == &other) return *this;
		calledCAs = true; 
		return *this;
	}
	Test &operator=(Test&& other) noexcept { 
		if(this == &other) return *this;
		calledMAs = true; 
		return *this;
	}
};

TEST_CASE( "ChunkedArraySemantics", "[ECS] [ChunkedArray]") {
	Core::ChunkedArray<Test> ca;

	SECTION("Adding") {
		auto _ = GENERATE(range(1, 40));
		ca.emplace_back();
		REQUIRE(calledCon == true);
		calledCon = false;
		REQUIRE(calledDes == false);
		REQUIRE(calledCop == false);
		REQUIRE(calledMov == false);
		REQUIRE(calledCAs == false);
		REQUIRE(calledMAs == false);
	}

	SECTION("Deleting from back") {
		auto _ = GENERATE(range(1, 40));

		ca.pop_back();
		REQUIRE(calledDes == true);
		calledDes = false;
		REQUIRE(calledCon == false);
		REQUIRE(calledCop == false);
		REQUIRE(calledMov == false);
		REQUIRE(calledCAs == false);
		REQUIRE(calledMAs == false);
	}
}
