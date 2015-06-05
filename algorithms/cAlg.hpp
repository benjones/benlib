#pragma once

/*
  
  container versions of some stl algorithms
  so I don't have to keep typing .begin() ane .end(),
  and potentially mixing iterators

  I strongly debated calling these contAlgorthims, but
  I thought I would giggle too much at the resemblance to other
  C-words.

 */

#include <algorithm> //source of most of these
#include <numeric>

namespace benlib{
  namespace cAlg{
	
	template<typename Cont1, typename Cont2>
	bool equal(Cont1&& cont1, Cont2&& cont2){
	  return std::equal(std::begin(cont1), std::end(cont1),
		  std::begin(cont2));
	}	
	
	template<typename Cont, typename OutputIt>
	OutputIt copy(Cont&& cont, OutputIt out){
	  return std::copy(std::begin(cont), std::end(cont), out);
	}

	template<typename Cont, typename OutputIt, typename Pred>
	OutputIt copy_if(Cont&& cont, OutputIt out, Pred&& pred){
	  return std::copy_if(std::begin(cont), std::end(cont), out, std::forward<Pred>(pred));
	}


	//essentially a specialized accumulate
	template<typename Cont, typename Extractor, 
			 typename Res = typename std::result_of<
			   Extractor(typename Cont::value_type)>::type
			 >
	Res sum_feature(Cont&& cont, Extractor&& ex, Res init){
	  using std::begin;
	  using std::end;
	  auto start = begin(cont);
	  auto stop = end(cont);

	  for(; start != stop; ++start){
		init += ex(*start);
	  }
	  return init;
	}

	

  }

}
