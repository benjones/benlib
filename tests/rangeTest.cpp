#define CATCH_CONFIG_MAIN  
#include "catch.hpp"

#include "../types/range.hpp"
#include <vector>
#include <algorithm>
#include <numeric>

using benlib::range;

TEST_CASE("one arg tests"){
  
  auto r = range(10);
  REQUIRE( std::count_if(r.begin(), r.end(), [](int){return true;}) == 10);
  REQUIRE( r.begin()[0] == 0);
  REQUIRE( r.begin()[8] == 8);
  REQUIRE( std::distance(r.begin(), r.end()) == 10);
}


TEST_CASE("two arg tests"){

  auto r = range(3, 5);
  REQUIRE(std::distance(r.begin(), r.end()) == 2);
  REQUIRE(std::accumulate(r.begin(), std::end(r), 0) == 7);
}

TEST_CASE("3 arg tests"){
  auto r = range(0, 10, 2);
  std::vector<int> res;
  std::adjacent_difference(std::begin(r), std::end(r), std::back_inserter(res));
  REQUIRE(std::all_of(res.begin() + 1, res.end(), [](int a){return a == 2;}));
  REQUIRE(*(r.end() - 1) == 8);
  REQUIRE(*(r.end()) == 10);
}

TEST_CASE("negative step"){
  auto r = range(10, 0, -1);
  REQUIRE(*r.begin() == 10);
  REQUIRE(*r.end() == 0);
  REQUIRE( std::count_if(r.begin(), r.end(), [](int){return true;}) == 10);
  REQUIRE( std::distance(r.begin(), r.end()) == 10);
}

TEST_CASE("count by twos"){
  auto r = range(0, 10, 2);
  REQUIRE(*r.begin() == 0);
  REQUIRE(*(r.begin() + 1) == 2);
  REQUIRE(*(r.end()) == 10);
  REQUIRE(std::distance(r.begin(), r.end()) == 5);
  REQUIRE(std::count_if(r.begin(), r.end(), [](int){return true;}) == 5);
}

TEST_CASE("doubles"){
  auto r = range(0.0, 10.0, 2.0);
  REQUIRE(*r.begin() == 0.0);
  REQUIRE(*(r.begin() + 1) == 2.0);
  REQUIRE(*(r.end()) == 10.0);
  REQUIRE(std::distance(r.begin(), r.end()) == 5);
  REQUIRE(std::count_if(r.begin(), r.end(), [](int){return true;}) == 5);
  
}

TEST_CASE("negative step, doubles"){
  auto r = range(10.0, 0.0, -1.0);
  REQUIRE(*r.begin() == 10.0);
  REQUIRE(*r.end() == 0.0);
  REQUIRE( std::count_if(r.begin(), r.end(), [](int){return true;}) == 10);
  REQUIRE( std::distance(r.begin(), r.end()) == 10);
}
