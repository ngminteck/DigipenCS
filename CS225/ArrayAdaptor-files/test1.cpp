#include <algorithm>
#include <vector>
#include <iterator>
#include <memory>
#include <cstdlib>
#include <string>
#include "ArrayAdaptor.h"

template<typename ValueType, typename OutputIterator>
void init_container(OutputIterator out, size_t sz)
{
  ValueType x {ValueType()}; /* default initialize x*/
  auto inc = [&x]() {
    return ++x;
  };
  std::generate_n(out, sz, inc);
}

template<typename RetType, typename Array,
         typename FirstDim,  typename ... RestDims>
RetType multiple_subscript(Array&& a, FirstDim index, RestDims ... rest_indices)
{
  return multiple_subscript<RetType>(a[index], rest_indices...);
}


template<typename RetType, typename Array, typename FirstDim>
RetType multiple_subscript(Array&& a, FirstDim index)
{
  return a[index];
}

template<typename RetType, typename ArrayCont, typename ... MaxDims>
RetType AccessRandomElement(ArrayCont && cont, MaxDims ... max_dims)
{
  return multiple_subscript<RetType>(std::forward<ArrayCont>(cont),
                                     (rand()%max_dims)...);
}

template<unsigned ... DimSizes>
void RandomPointerTest(int number_of_test, int num_of_elements)
{
  std::unique_ptr<int[]> up(new int[num_of_elements]);
  init_container<int>(up.get(), num_of_elements);
  ArrayAdaptor<int, DimSizes...> adapted_sp(up.get());
  for(unsigned i=0; i<number_of_test; ++i)
    std::cout << AccessRandomElement<int>(adapted_sp, 5,5,2) << ' ';
  std::cout << std::endl;
}

void Test1()
{
  std::string x[1] = {"abc"};
  ArrayAdaptor<std::string, 1> adapted_x(x);
  std::cout << adapted_x[0] << std::endl;
  std::cout << AccessRandomElement<std::string>(adapted_x, 1);
}

void Test2()
{
  std::string x[2] = {"abc", "def"};
  ArrayAdaptor<std::string, 2> adapted_x(x);
  std::cout << adapted_x[1] << std::endl;
  std::cout << AccessRandomElement<std::string>(adapted_x, 1);
  adapted_x[1]="ghi";
  std::cout << x[1] << std::endl;
}

void Test3()
{
  char c[8] { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  ArrayAdaptor<char, 2,2,2> adapted_c(c);
  for(unsigned i=0; i<2; ++i)
    for(unsigned j=0; j<2; ++j)
      for(unsigned k=0; k<2; ++k)
        std::cout << adapted_c[i][j][k];
  std::cout << std::endl;
  adapted_c[0][1][1]='x';
  std::cout << c[3] << std::endl;
}

int main()
{
  Test1();
  Test2();
  Test3();
}
