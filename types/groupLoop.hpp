#pragma once

#include "typeTraits.hpp"


/* take a range, return n elements at a time, looping around the end

 */



namespace benlib{

  namespace {

	template <typename Cont, typename Func, typename InitType, std::size_t ... AdvanceIndices>
	auto accumulateGroupsImpl(
		Cont&& c, Func&& f, InitType&& init, std::index_sequence<AdvanceIndices ...>){
	  
	  auto beginIt = std::begin(c), endIt = std::end(c);
	  
	  auto loopedAdvance = [beginIt, endIt](auto it, std::size_t amount){
		std::size_t distanceToEnd = std::distance(it, endIt);
		if(amount < distanceToEnd){
		  return it + amount;
		} else {
		  return beginIt + amount - distanceToEnd;
		}
	  };
	  
	  for(auto it = beginIt; it != endIt; ++it){
		init = applyToTuple(std::forward<Func>(f), 
			tuple_cat(std::tie(init),  
				std::forward_as_tuple( *loopedAdvance(it, AdvanceIndices)...)));
	  }
	  return init;
	}
  }


  template<typename Cont, typename Func, std::size_t ... Indices>
  auto accumulateGroups(Cont&& c, Func&& f, typename function_traits<Func>::return_type init = {}){

	using InitType = typename function_traits<Func>::return_type;
	auto constexpr arity = function_traits<Func>::arity - 1;
	using countSequence = std::make_index_sequence<arity>;
	return accumulateGroupsImpl(
		std::forward<Cont>(c), std::forward<Func>(f), std::forward<InitType>(init), countSequence{});
	
  }

}
