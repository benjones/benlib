#include "../types/preallocVector.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>

template<typename T>
void dump(const T& t){
  std::cout << "container dump: " << std::endl;
  for(auto&& u : t){
	std::cout << u << std::endl;
  }
}

int main(){

  benlib::PreallocVector<double,3> pvec;
  
  pvec.push_back(1);
  pvec.push_back(2);
  pvec.push_back(3);
  pvec.push_back(4);

  pvec.push_back(20);  
  pvec.push_back(15);
  pvec.push_back(-20);
  pvec.push_back(0);
  pvec.push_back(13);
  pvec.emplace_back(5);
  

  for(auto d : pvec){
	std::cout << d << std::endl;
  }
  //  const benlib::PreallocVector<double> pvec2;
  //  auto beg2 = pvec2.begin();
  std::sort(pvec.begin(), pvec.end());
  for(auto d : pvec){
	std::cout << d << std::endl;
  }

  
  //assign tests
  {
	std::cout << "assign test 1: " << std:: endl;
	std::vector<int> sv{0,1,2,3,4,5};
	
	benlib::PreallocVector<int> pv;
	pv.assign(sv.begin(), sv.end());
	dump(pv);
	assert(pv.size() == sv.size());
  }

  {
	std::cout << "assign test 2: " << std:: endl;
	std::vector<int> sv{0,1,2,3,4,5};
	
	benlib::PreallocVector<int, 3> pv;
	pv.assign(sv.begin(), sv.end());
	dump(pv);
	assert(pv.size() == sv.size());
  }

  {
	std::cout << "assign test 3: " << std:: endl;
	std::vector<int> sv{0,1,2,3,4,5};
	
	benlib::PreallocVector<int, 3> pv;
	pv.push_back(3);
	pv.assign(sv.begin(), sv.end());
	dump(pv);
	assert(pv.size() == sv.size());
  }

  {
	std::cout << "assign test 4: " << std:: endl;
	benlib::PreallocVector<int, 3> pv;
	pv.push_back(3);
	pv.assign(10ul, 4);
	dump(pv);
	assert(pv.size() == 10);
  }
  {
	std::cout << "assign test 5: " << std:: endl;
	benlib::PreallocVector<int, 3> pv;
	pv.assign(10ul, 4);
	benlib::PreallocVector<int> pv2(pv);
	dump(pv2);
	assert(pv.size() == pv2.size());
  }


}
