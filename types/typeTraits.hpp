#pragma once

//some handy type trait stuff

namespace benlib{

  namespace {
	template <typename Fnc, typename ... Types, std::size_t ... Indices>
	auto apply_impl(Fnc &&fnc, const std::tuple<Types...> &tuple, std::index_sequence<Indices...>)
	{
	  return std::forward<Fnc>(fnc)(std::get<Indices>(tuple)...);
	}
  }
  
  template <typename Fnc, typename ... Types>
  auto applyToTuple(Fnc &&fnc, const std::tuple<Types...> &tuple)
  {
	return apply_impl(std::forward<Fnc>(fnc), tuple, std::index_sequence_for<Types...>());
  }


  //https://functionalcpp.wordpress.com/2013/08/05/function-traits/
  template<class F>
  struct function_traits;
  
  // function pointer
  template<class R, class... Args>
  struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)>
  {};
 
  template<class R, class... Args>
  struct function_traits<R(Args...)>
  {
    using return_type = R;
 
    static constexpr std::size_t arity = sizeof...(Args);
 
    template <std::size_t N>
    struct argument
    {
	  static_assert(N < arity, "error: invalid parameter index.");
	  using type = typename std::tuple_element<N,std::tuple<Args...>>::type;
    };
  };

  // member function pointer
  template<class C, class R, class... Args>
  struct function_traits<R(C::*)(Args...)> : public function_traits<R(C&,Args...)>
  {};
  
  // const member function pointer
  template<class C, class R, class... Args>
  struct function_traits<R(C::*)(Args...) const> : public function_traits<R(C&,Args...)>
  {};
  
  
  // member object pointer
  template<class C, class R>
  struct function_traits<R(C::*)> : public function_traits<R(C&)>
  {};

  // functor
  template<class F>
  struct function_traits
  {
  private:
	using call_type = function_traits<decltype(&F::operator())>;
  public:
	using return_type = typename call_type::return_type;
	
	static constexpr std::size_t arity = call_type::arity - 1;
	
	template <std::size_t N>
	struct argument
	{
	  static_assert(N < arity, "error: invalid parameter index.");
	  using type = typename call_type::template argument<N+1>::type;
	};
  };
  
  template<class F>
  struct function_traits<F&> : public function_traits<F>
  {};
  
  template<class F>
  struct function_traits<F&&> : public function_traits<F>
  {};
  

}
