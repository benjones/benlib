#pragma once
/*
This is a scalar class templated on type.  

I wrote it mostly to change default initalization of scalars to either
1.  Make creating a std::vector faster (w/ a donothing constructor)
2.  Make unitialized values pop (by setting it to NaN)

1 would be for optimized, release builds,
2 would be for debug builds (although I suspect overhead is very low)
*/

#include <limits>

namespace benlib{
  
  template <typename T>
  class ScalarTBase{	
  public:
	typedef T value_type;

		//increment ops
	ScalarTBase& operator++()
	{
	  ++data;
	  return *this;
	}
	const ScalarTBase operator++(int)
	{
	  ScalarTBase tmp{*this};
	  operator++();
	  return tmp;
	}

	//unary arithmetic ops
	ScalarTBase& operator += (ScalarTBase other){
	  this->data += other.data;
	  return *this;
	}

	ScalarTBase& operator -= (ScalarTBase other){
	  this->data -= other.data;
	  return *this;
	}

	ScalarTBase& operator *= (ScalarTBase other){
	  this->data *= other.data;
	  return *this;
	}

	ScalarTBase& operator /= (ScalarTBase other){
	  this->data /= other.data;
	  return *this;
	}

	ScalarTBase operator +(){
	  return *this;
	}

	
	T data;
	
  protected: 
	ScalarTBase() {}
	ScalarTBase(T t) : data(t) {}
  };
	

  //the actual types:
  template<typename T, bool safe>
  class ScalarT : public ScalarTBase<T> {
	public:
	ScalarT() : ScalarTBase<T>{std::numeric_limits<T>::quiet_NaN()} {} 
	explicit ScalarT(T t) : ScalarTBase<T>{t} {}
	ScalarT(const ScalarT& other) = default;
	ScalarT(ScalarT&& other) = default;
	ScalarT& operator=(const ScalarT& other) = default;
	ScalarT& operator=(ScalarT&& other) = default;
	ScalarT operator -(){
	  return ScalarT(-(this->data));
	}

  };

  template<typename T>
  class ScalarT<T, false> : public ScalarTBase<T> {
	public:
	ScalarT(){} //no initialization
	explicit ScalarT(T t) : ScalarTBase<T>{t} {}
	ScalarT(const ScalarT& other) = default;
	ScalarT(ScalarT&& other) = default;
	ScalarT& operator=(const ScalarT& other) = default;
	ScalarT& operator=(ScalarT&& other) = default;
	ScalarT operator -(){
	  return ScalarT(-(this->data));
	}
  };

	//comparison operators
	
  template<typename T, bool safe>
  inline bool operator==(const ScalarT<T, safe>& lhs, const ScalarT<T, safe>& rhs){ return lhs.data == rhs.data; }
  template<typename T, bool safe>
  inline bool operator!=(const ScalarT<T, safe>& lhs, const ScalarT<T, safe>& rhs){return !operator==(lhs,rhs);}
  template<typename T, bool safe>
  inline bool operator< (const ScalarT<T, safe>& lhs, const ScalarT<T, safe>& rhs){ return lhs.data < rhs.data; }
  template<typename T, bool safe>
  inline bool operator> (const ScalarT<T, safe>& lhs, const ScalarT<T, safe>& rhs){return  operator< (rhs,lhs);}
  template<typename T, bool safe>
  inline bool operator<=(const ScalarT<T, safe>& lhs, const ScalarT<T, safe>& rhs){return !operator> (lhs,rhs);}
  template<typename T, bool safe>
  inline bool operator>=(const ScalarT<T, safe>& lhs, const ScalarT<T, safe>& rhs){return !operator< (lhs,rhs);}
  
  
  template<typename T, bool safe>
  ScalarT<T, safe> operator +(ScalarT<T, safe> lhs, const ScalarT<T, safe>& rhs){
	lhs += rhs;
	return lhs;
  }
  template<typename T, bool safe>
  ScalarT<T, safe> operator -(ScalarT<T, safe> lhs, const ScalarT<T, safe>& rhs){
	lhs -= rhs;
	return lhs;
  }
  template<typename T, bool safe>
  ScalarT<T, safe> operator *(ScalarT<T, safe> lhs, const ScalarT<T, safe>& rhs){
	lhs *= rhs;
	return lhs;
  }
  template<typename T, bool safe>
  ScalarT<T, safe> operator /(ScalarT<T, safe> lhs, const ScalarT<T, safe>& rhs){
	lhs /= rhs;
	return lhs;
  }  

  //in order from CMATH:

  template<typename T, bool safe>
  inline ScalarT<T, safe> abs(ScalarT<T, safe> t){
	return ScalarT<T, safe>(abs(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> fabs(ScalarT<T, safe> t){
	return ScalarT<T, safe>(fabs(t.data));
  }

  //skipping all the new c++11 functions
  
  template<typename T, bool safe>
  inline ScalarT<T, safe> exp(ScalarT<T, safe> t){
	return ScalarT<T, safe>(exp(t.data));
  }

  template<typename T, bool safe>
  inline ScalarT<T, safe> log(ScalarT<T, safe> t){
	return ScalarT<T, safe>(log(t.data));
  }

  template<typename T, bool safe>
  inline ScalarT<T, safe> log10(ScalarT<T, safe> t){
	return ScalarT<T, safe>(log10(t.data));
  }

  template<typename T, bool safe>
  inline ScalarT<T, safe> sqrt(ScalarT<T, safe> t){
	return ScalarT<T, safe>(sqrt(t.data));
  }

  template<typename T, bool safe>
  inline ScalarT<T, safe> pow(ScalarT<T, safe> x, ScalarT<T, safe> y){
	return ScalarT<T, safe>(pow(x.data, y.data));
  }
  template<typename T, bool safe, typename U>
  inline ScalarT<T, safe> pow(ScalarT<T, safe> x, U y){
	return ScalarT<T, safe>(pow(x.data, y));
  }
  //trig functions
  template<typename T, bool safe>
  inline ScalarT<T, safe> sin(ScalarT<T, safe> t){
	return ScalarT<T, safe>(sin(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> cos(ScalarT<T, safe> t){
	return ScalarT<T, safe>(cos(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> tan(ScalarT<T, safe> t){
	return ScalarT<T, safe>(tan(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> asin(ScalarT<T, safe> t){
	return ScalarT<T, safe>(asin(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> acos(ScalarT<T, safe> t){
	return ScalarT<T, safe>(acos(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> atan(ScalarT<T, safe> t){
	return ScalarT<T, safe>(log(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> atan2(ScalarT<T, safe> t, ScalarT<T, safe> u){
	return ScalarT<T, safe>(atan2(t.data, u.data));
  }
  //hyperbolic trig functions
  template<typename T, bool safe>
  inline ScalarT<T, safe> sinh(ScalarT<T, safe> t){
	return ScalarT<T, safe>(sinh(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> cosh(ScalarT<T, safe> t){
	return ScalarT<T, safe>(cosh(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> tanh(ScalarT<T, safe> t){
	return ScalarT<T, safe>(tanh(t.data));
  }
  //rounding
  template<typename T, bool safe>
  inline ScalarT<T, safe> floor(ScalarT<T, safe> t){
	return ScalarT<T, safe>(floor(t.data));
  }
  template<typename T, bool safe>
  inline ScalarT<T, safe> ceil(ScalarT<T, safe> t){
	return ScalarT<T, safe>(ceil(t.data));
  }
  
  //skipping some of the weirder ones

  typedef ScalarT<double, false> scalar_t;
  
}
