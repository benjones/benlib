#define CATCH_CONFIG_MAIN  
#include "catch.hpp"

#include "../algorithms/stdPlus.hpp"
#include "../types/range.hpp"
#include <vector>
#include <algorithm>

TEST_CASE("transform_if"){
  auto r = benlib::range(10);
  std::vector<int> result;
  benlib::stdPlus::transform_if(r.begin(), r.end(),
	  std::back_inserter(result),
	  [](int a){return a*a;},
	  [](int a){return a % 2 == 0;});
  
  auto expected = {0,4,16,36,64};
  REQUIRE(std::equal(result.begin(), result.end(), expected.begin()));
  
  //works with set?
  std::set<int> setInput{r.begin(), r.end()};
  result.clear();
  benlib::stdPlus::transform_if(setInput.begin(), setInput.end(),
	  std::back_inserter(result),
	  [](int a){return a*a;},
	  [](int a){return a % 2 == 0;});
  REQUIRE(std::equal(result.begin(), result.end(), expected.begin()));
  
}
