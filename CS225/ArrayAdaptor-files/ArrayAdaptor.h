#include <iostream>
#include "SingleArray.h"

template <unsigned... rest_dim_size_num>
struct ComputeRestDimSizes;

template <unsigned first_dim_size_num, unsigned... rest_dim_size_num>
struct ComputeRestDimSizes<first_dim_size_num, rest_dim_size_num...>
{
  static unsigned value()
  {
    return first_dim_size_num * 
    ComputeRestDimSizes<rest_dim_size_num...>::value();
  }  
};

template <unsigned first_dim_size_num>
struct ComputeRestDimSizes<first_dim_size_num>
{
  static unsigned value()
  {
    return first_dim_size_num;
  }    
};


template< typename T , unsigned FirstDimSize , unsigned ... RestDimSizes>
class ArrayAdaptor
{
  SingleArray<T> single_array;
  unsigned infront_array_index;
    
  public :
    using SubscriptResultType = ArrayAdaptor<T , RestDimSizes ... >; 
    
    ArrayAdaptor():
    single_array(SingleArray<T>()),
    infront_array_index(unsigned())
    {
    
    }
    
    template<typename U,typename V = typename 
      std::enable_if_t<
        !std::is_same<
          std::decay_t<U>
          ,ArrayAdaptor>
        ::value>
      >
    ArrayAdaptor(U&& t)
    :single_array(std::forward<U>(t)),infront_array_index(unsigned())          
    {
    
    }
 
    ArrayAdaptor(const SingleArray<T>& array, unsigned index)
    :single_array(array),infront_array_index(index)
    {
    
    }
  
    SubscriptResultType operator [] (int index) 
    {
       return SubscriptResultType(single_array, infront_array_index + index * 
                                ComputeRestDimSizes<RestDimSizes...>::value() );
    }
    SubscriptResultType operator [] (int index) const 
    {
      return SubscriptResultType(single_array, infront_array_index + index * 
                                ComputeRestDimSizes<RestDimSizes...>::value() );
    }
};

template< typename T , unsigned OnlyOneDim>
class ArrayAdaptor<T , OnlyOneDim>
{
  SingleArray<T> single_array;
  unsigned infront_array_index;
  
  public:
    ArrayAdaptor()
    :single_array(SingleArray<T>()),infront_array_index(unsigned())
    {
    
    }
    template<typename U,typename V = typename 
      std::enable_if_t<
        !std::is_same<
          std::decay_t<U>
          ,ArrayAdaptor>
        ::value>
      >
    ArrayAdaptor(U&& t)
    :single_array(std::forward<U>(t)),infront_array_index(unsigned())          
    {
    
    }
    
    ArrayAdaptor(const SingleArray<T>& array, unsigned index)
    :single_array(array),infront_array_index(index)
    {
    
    }

    T& operator[](int index)
    {
      return single_array.operator[](infront_array_index + index);
    }
    
    T operator[](int index)const
    {
     return single_array.operator[](infront_array_index + index);
    }
   
};