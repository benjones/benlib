#pragma once

/*

stdPlus:  algorithms that should be in the STL, but aren't


 */

namespace benlib{
  namespace stdPlus{
	/*
	  write transformed version of input range, skipping any elements that
	  don't satisfy the predicate.
	*/
	template <typename InputIt, typename OutputIt,
			  typename UnaryFunc, typename Predicate>
	OutputIt transform_if(InputIt start, InputIt end,
		OutputIt out,
		UnaryFunc&& func,
		Predicate&& pred){
	  
	  while(start != end){
		if(pred(*start)){
		  out++ = func(*start);
		}
		++start;
	  }
	  return out;
	}
	
  }
}
