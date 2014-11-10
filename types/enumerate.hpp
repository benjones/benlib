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
		return (lhs.rangeIter == rhs.rangeIter);
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
	  
	  //dereference operators
	  const std::pair<size_t, typename T::reference>&
	  operator *() const{
		return std::pair<size_t, typename T::reference>{*rangeIter, *containerIter};
	  }
	  //non-const reference to container entry
	  std::pair<size_t, typename T::reference>
	  operator *(){
		return std::pair<size_t, typename T::reference>{*rangeIter, *containerIter};
	  }

	  //don't include operator ->... not sure what it would look like
	  
	  template <typename U = T>
	  typename std::enable_if<std::is_base_of<std::random_access_iterator_tag,
											  typename U::iterator::iterator_category>::value,
							  const std::pair<size_t, typename T::reference> >::type 
	  operator [](size_t n) const{
		return std::pair<size_t, typename T::reference>{rangeIter[n], containerIter[n]};
	  }

	  template <typename U = T>
	  typename std::enable_if<std::is_base_of<std::random_access_iterator_tag,
											  typename U::iterator::iterator_category>::value,
							  std::pair<size_t, typename T::reference> >::type 
	  operator [](size_t n) {
		return std::pair<size_t, typename T::reference>{rangeIter[n], containerIter[n]};
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
	  end_{benlib::FastRange<size_t>::iterator{container.size()},
		  //std::distance(std::begin(container), //I assume size is faster than distance?
		  //			 std::end(container))},
		  std::end(container)}
	{}
	
	using iterator = Iterator;
	using const_iterator = Iterator;
	Iterator begin() const {return begin_;}
	Iterator end() const {return end_;}
	
	Iterator begin() { return begin_;}
	Iterator end() { return end_;}

  private:
	const Iterator begin_, end_;

  };
  

  template<typename T>
  Enumerate<T> enumerate(T& t){ return Enumerate<T>{t};}

}
