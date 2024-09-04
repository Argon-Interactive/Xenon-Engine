#include <catch2/catch_test_macros.hpp>
#include "ECS/entity.hpp"

TEST_CASE( "test", "[test]" ) {
	Core::Entity a = 13840910;
    REQUIRE( a == 17 );
}
