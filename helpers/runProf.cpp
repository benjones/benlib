#include <iostream>
#include "profiler.hpp"
int main(){
  benlib::Profiler prof;

  {
	auto t1 = prof.timeName("loop1");
	double tot = 0;
	for(auto i = 0; i < 10000; i++){
	  for(auto j = 0; j < 10000; j++){
		tot += i*j;
	  }
	}
	std::cout << tot << std::endl;
  }
  {
	auto t1 = prof.timeName("loop2");
	double tot = 0;
	for(auto i = 0; i < 10000; i++){
	  auto t3 = prof.timeName("loop3");
	  for(auto j = 0; j < 10000; j++){
		tot += i*j;
	  }
	}
	std::cout << tot << std::endl;
  }
  {
	auto t1 = prof.timeName("loop4");
	double tot = 0;
	for(auto i = 0; i < 10000; i++){
	  auto t3 = prof.timeName("loop5");
	  for(auto j = 0; j < 10000; j++){
		tot += i*j;
	  }
	}
	std::cout << tot << std::endl;
  }
  {
	auto t1 = prof.timeName("loop6");
	double tot = 0;
	for(auto i = 0; i < 10000; i++){
	  for(auto j = 0; j < 10000; j++){
		tot += i*j;
	  }
	}
	std::cout << tot << std::endl;
  }
  
 prof.dump<std::chrono::duration<double>>(std::cout);
 prof.dump<std::chrono::nanoseconds>(std::cout);
 
}
