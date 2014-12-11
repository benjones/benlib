#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../algorithms/cAlg.hpp"
#include <vector>

TEST_CASE("equal"){
  const auto input = {0,1,2,3,4,5};
  REQUIRE(benlib::cAlg::equal(input, input));
  const std::vector<int> inputVec = {0,1,2,3,4,5};
  REQUIRE(benlib::cAlg::equal(input, inputVec));
}

TEST_CASE("copy"){
  
  const auto input = {0,1,2,3,4,5};
  std::vector<int> output;
  benlib::cAlg::copy(input, std::back_inserter(output));
  REQUIRE(benlib::cAlg::equal(input, output));
}

TEST_CASE("copy_if"){
  const auto input = {0,1,2,3,4,5};
  std::vector<int> output;
  benlib::cAlg::copy_if(input, std::back_inserter(output),
	  [](int a){return a %2 == 0;});
  auto expectedOutput = {0,2,4};
  REQUIRE(benlib::cAlg::equal(output, expectedOutput));
  

}

