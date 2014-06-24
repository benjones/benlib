#define CATCH_CONFIG_MAIN  
#include "catch.hpp"

#include "../types/preallocVector.hpp"
#include <vector>
#include <algorithm>

TEST_CASE("works like vector"){

  benlib::PreallocVector<int,3> pvec;
  std::vector<int> svec;

  pvec.push_back(1); svec.push_back(1);
  pvec.push_back(2); svec.push_back(2);
  pvec.push_back(3); svec.push_back(3);
  pvec.push_back(4); svec.push_back(4);

  pvec.push_back(20); svec.push_back(20);
  pvec.push_back(15); svec.push_back(15);
  pvec.push_back(-20); svec.push_back(-20);
  pvec.push_back(0); svec.push_back(0);
  pvec.push_back(13); svec.push_back(13);
  pvec.emplace_back(5); svec.emplace_back(5);
  
  REQUIRE(pvec.size() == svec.size());
  REQUIRE(std::equal(pvec.begin(), pvec.end(), svec.begin()));

  SECTION("Can be sorted like a std::vector"){
	std::sort(pvec.begin(), pvec.end());
	std::sort(svec.begin(), svec.end());
	REQUIRE(std::equal(pvec.begin(), pvec.end(), svec.begin()));
  }
}
  
TEST_CASE("assign works properly"){
  //assign tests
  
  SECTION("assign from vector"){
	std::vector<int> sv{0,1,2,3,4,5};
	benlib::PreallocVector<int> pv;
	pv.assign(sv.begin(), sv.end());
	REQUIRE(std::equal(pv.begin(), pv.end(), sv.begin()));
  }

  SECTION("assign, overslowing static storage"){
	std::vector<int> sv{0,1,2,3,4,5};
	benlib::PreallocVector<int, 3> pv;
	pv.assign(sv.begin(), sv.end());
	REQUIRE(std::equal(pv.begin(), pv.end(), sv.begin()));
  }

  SECTION("assign overwrites contents"){
	std::vector<int> sv{0,1,2,3,4,5};
	benlib::PreallocVector<int, 3> pv;
	pv.push_back(3);
	pv.assign(sv.begin(), sv.end());
	REQUIRE(std::equal(pv.begin(), pv.end(), sv.begin()));
  }

  SECTION("assign with value and count works"){
	benlib::PreallocVector<int, 3> pv;
	pv.push_back(3);
	pv.assign(10ul, 4);
	REQUIRE(pv.size() == 10);
  }

  SECTION("copy constructor"){
	benlib::PreallocVector<int, 3> pv;
	pv.assign(10ul, 4);
	benlib::PreallocVector<int> pv2(pv);
	REQUIRE(std::equal(pv.begin(), pv.end(), pv2.begin()));
  }
  SECTION("push back changes contents"){
	std::vector<int> sv{0,1,2,3,4,5};
	benlib::PreallocVector<int, 3>pv(sv.begin(), sv.end());
	pv.push_back(3);
	REQUIRE(sv.size() != pv.size());
  }


}
