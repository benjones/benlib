#define CATCH_CONFIG_MAIN  
#include "catch.hpp"

#include "../types/enumerate.hpp"
#include <vector>
#include <algorithm>
#include <numeric>

using benlib::enumerate;

TEST_CASE("tests"){
  std::vector<int> v{0,1,2,3};
  enumerate(v);
  REQUIRE(true);
}
