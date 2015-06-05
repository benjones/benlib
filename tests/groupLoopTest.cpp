#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../types/groupLoop.hpp"
#include <vector>

TEST_CASE("vecOfInts"){
  
  std::vector<int> v {1,2,3,4,5};

  auto lambda2 = [](int acc, int a, int b){return acc + a*b;};

  //without init
  REQUIRE(benlib::accumulateGroups(v, lambda2) ==
	   2 + 6 + 12 + 20 + 5);
  

  //with init  
  REQUIRE(benlib::accumulateGroups(v, lambda2, 10) ==
	  10 + 2 + 6 + 12 + 20 + 5);


  //length 3:
  auto lambda3 = [](int acc, int a, int b, int c){return acc + a*b*c;};

  //without init
  REQUIRE(benlib::accumulateGroups(v, lambda3) ==
	  6 + 24 + 60 + 20 + 10);
  

  //with init  
  REQUIRE(benlib::accumulateGroups(v, lambda3, 10) ==
	  10 + 6 + 24 + 60 + 20 + 10);

  

}
