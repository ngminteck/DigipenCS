#include <iostream>
#include <string> 
#include <tuple>

template<typename... Types>
class Tuple;

//recursive case
template<typename Head, typename...Tail>
class Tuple<Head,Tail...>
{
  private:
    Head head;
    Tuple<Tail...> tail;
  public:
    
    Tuple()
    {
    
    }
    
    Tuple(Head const& head, Tuple<Tail...> const& tail)
    :head(head),tail(tail)
    {
      
    }
    // copy constructor
    Tuple(Head const& head, Tail const&... tail)
    :head(head),tail(tail...)
    {
      
    }
    // move constructor
    template<typename VHead, typename...VTail,
             typename std::enable_if_t<sizeof...(VTail)==sizeof...(Tail)>>
    Tuple(VHead&& vhead, VTail&&...vtail)
    :head(std::forward<VHead>(vhead)), tail(std::forward<VTail>(vtail)...)
    {
      
    }
    
    template<typename VHead, typename...VTail,
             typename std::enable_if_t<sizeof...(VTail)==sizeof...(Tail)>>
    Tuple(Tuple<VHead,VTail...>const & other)
    :head(other.getHead()),tail(other.getTail())
    {
      
    }
    
    Head& getHead()
    {
      return head;
    }
    
    Head const& getHead() const
    {
      return head;
    }
    
    Tuple<Tail...>& getTail()
    {
      return tail;
    }
    
    Tuple<Tail...> const& getTail() const
    {
      return tail;
    }
};

//basic case
template<>
class Tuple<>
{
  // no storage
};

// get tuple
 template <unsigned N>
struct TupleGet
{
  template<typename Head, typename... Tail>
  static auto apply(Tuple<Head, Tail...> const& t)
  {
    return TupleGet<N - 1>::apply(t.getTail());
  }
};

template<>
struct TupleGet<0>
{
  template<typename Head, typename... Tail>
  static Head const& apply(Tuple<Head, Tail...> const& t)
  {
    return t.getHead();
  }
};

template <unsigned N, typename... Types>
auto get(Tuple<Types...> const& t)
{
  return TupleGet<N>::apply(t);
}

/***edward version***/
template<typename ... Types>
struct RecTuple;

template<typename Head, typename ... Tail>
struct RecTuple<Head, Tail...> 
/* : RecTuple<Types...>  affects memory layout*/ 
{
     Head value; 
	 RecTuple<Tail...> sub_tuple; 
};

template<>
struct RecTuple<>
{
	
};
template<unsigned Index, typename ... Types>
// 1 or more template parameters here. 
// 1 non-type parameter
// 1 type parameter pack 
auto MyGet(const RecTuple<Types...>& tuple)
{
	//if constexpr C++17 only. 
	//it's still a programming statement 
	// so it must be inside a function or a body. 
     if constexpr (Index==0)
           return tuple.value; 
     else
           return MyGet<Index-1>(tuple.sub_tuple);
}

// make tuple
template<typename...Types>
auto makeTuple(Types&&... elems)
{
  return Tuple<std::decay_t<Types>...>(std::forward<Types>(elems)...);
}

int main()
{
  Tuple<int, double, std::string> t(17,3.14,"Hello World!");
  std::cout << get<0>(t) << std::endl; 
  RecTuple<std::string, int , char> t_edward { "abcde",5, 'x'};
  std::cout << MyGet<1>(t_edward) << std::endl;


  //Tuple<long int, long double, std::string> t2(t);
  
  //makeTuple(17,3.14,"Hello World");
}