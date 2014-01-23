
#include <numeric>
#include "../helpers/portableBlas.hpp"

namespace benlib {
  template <typename C>
  C::value_type
  sum(const C& container){
	return
	  std::accumulate(begin(container), end(container),
					  C::value_type{0.0});
  }



  //This is basically an inner_product + a sum, 
  //but the loops have been fused
  template <typename C, typename W>
  C::value_type 
  weightedAverage(const C& container, const W& weights){
	
	assert(container.size() == weights.size());
	
	auto weightTotal = W::value_type{0.0};
	auto accum = C::value_type{0.0};
	
	auto cIt = begin(container);
	auto wIt = begin(weights);
	auto cEnd = end(container);
	
	for(; cIt != cEnd; ++cIt, ++wIt){
	  accum += (*wIt)*(*cIt);
	  weightTotal += (*wIt);
	}
	
	accum *= (1.0/weightTotal);
	return accum;
  }
