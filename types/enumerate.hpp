#include "range.hpp"


namespace benlib{

  template<typename T>
  class Enumerate{
  public:
	class Iterator :
	  public std::iterator<typename T::iterator::iterator_category, 
						   std::pair<size_t, typename T::value_type> >{

	  
	public:
	  friend inline bool operator ==(const Iterator& lhs, const Iterator& rhs){
		//just check the range iter since the public interface doesn't let other stuff get weird
		return (lhs.rangeIter != rhs.rangeIter);
	  }
	  friend inline bool operator !=(const Iterator& lhs, const Iterator& rhs){
		return !(lhs == rhs);
	  }
	  friend inline bool operator <(const Iterator& lhs, const Iterator& rhs){
		return lhs.rangeIter < rhs.rangeIter;
	  }
	  friend inline bool operator <=(const Iterator& lhs, const Iterator& rhs){
		return lhs.rangeIter <= rhs.rangeIter;
	  }
	  friend inline bool operator >(const Iterator& lhs, const Iterator& rhs){
		return lhs.rangeIter > rhs.rangeIter;
	  }
	  friend inline bool operator >=(const Iterator& lhs, const Iterator& rhs){
		return lhs.rangeIter >= rhs.rangeIter;
	  }

	  Iterator& operator++() {
		++rangeIter;
		++containerIter;
		return *this;
	  }
	  Iterator operator++(int) {
		Iterator tmp{*this};
		operator ++();
		return tmp;
	  }
	  
	  template <typename U = T>
	  std::enable_if<std::is_base_of<std::bidirectional_iterator_tag,
									 typename U::iterator::iterator_category>::value::value,
					 Iterator&>
	  operator --(){
		--rangeIter;
		--containerIter;
		return *this;
	  }

	  Iterator operator --(int){
		Iterator tmp{*this};
		operator --();
		return tmp;
	  }
	  
	  Iterator(benlib::FastRange<size_t>::iterator i1,
			   typename T::iterator i2)
		: rangeIter{i1}, containerIter{i2}
	  {}

	private:
	  benlib::FastRange<size_t>::iterator rangeIter;
	  typename T::iterator containerIter;

	};


	Enumerate(T& container)
	  : begin_{benlib::FastRange<size_t>::iterator{0},
		  std::begin(container)},
	  end_{benlib::FastRange<size_t>::iterator{std::distance(std::begin(container),
															 std::end(container))},
		  std::end(container)}
	{}
	
	using iterator = Iterator;
	using const_iterator = Iterator;
	Iterator begin() const {return begin_;}
	Iterator end() const {return end_;}
	

  private:
	const Iterator begin_, end_;

  };
  

  template<typename T>
  Enumerate<T> enumerate(T& t){ return Enumerate<T>{t};}

};
