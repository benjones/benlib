#define CATCH_CONFIG_MAIN  
#include "catch.hpp"

#include "../types/enumerate.hpp"
#include <vector>
#include <algorithm>
#include <numeric>

using benlib::enumerate;

TEST_CASE("tests"){
  std::vector<size_t> v{0,1,2,3};

  for(auto&& pr : enumerate(v)){
	REQUIRE(pr.first == pr.second);
  }
  
  for(auto pr : enumerate(v)){
	++pr.second;
	REQUIRE((pr.first +1) == v[pr.first]);
	REQUIRE(pr.second == v[pr.first]);
  }

  const std::vector<size_t> cv{0,1,2,3,4,5};
  for(auto&& pr : enumerate(cv)){
	REQUIRE(pr.first == pr.second);
  }
  
  /*
  int a = 3, b = 5;
  std::pair<int&, int&> pr{a, b};

  REQUIRE(a == 3);
  pr.first = 4;
  REQUIRE(a == 4);

  int c = 10, d = 20;
  std::pair<int&, int&> pr2{c, d};
  std::swap(pr, pr2);
  REQUIRE(a == 10);
  REQUIRE(b == 20);
  REQUIRE(c == 4);
  REQUIRE(d == 5);
  */
}
