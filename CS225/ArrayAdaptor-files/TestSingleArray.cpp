#include <iostream> /* cout, cerr, cin*/
#include <string> /*std::string*/
#include <array> /*std::array*/
#include "SingleArray.h"

template<typename Cont>
void print_random_access_container(const std::string& heading, Cont c, int num)
{
  std::cout << heading;
  for(int i=0; i<num; ++i)
    std::cout << c[i] << ' ';
  std::cout << std::endl;
}

void LValueTest()
{
  std::string s {"abcd"};
  SingleArray<char> adapted_s {s};
  std::cout << adapted_s[3] << std::endl; /* prints 'd' */
  adapted_s[2]='f'; /* s is now "abfd" */
  s = "hello world";
  std::cout << adapted_s[3] << std::endl; /* prints 'l' */
}

void RValueTest()
{
  std::string s {"abcd"};
  SingleArray<char> adapted_s {std::move(s)};
  std::cout << adapted_s[3] << std::endl; /* prints 'd' */
  adapted_s[2]='f'; /* s is now "abfd" */
  s = "hello world";
  std::cout << adapted_s[3] << std::endl; /* prints 'd' */
}
//#if 0
void CopyControlTest()
{
  std::cout << "\n*************CopyControlTest****************\n";
  std::cout << "Copy ctor tests:\n";
  std::array<short, 10> short_array { 1,2,3,4,5,6,7,8,9,10};
  SingleArray<short> adapted_short_array1 { short_array };
  SingleArray<short> adapted_short_array2 { adapted_short_array1 };
  adapted_short_array1[4]=50;
  print_random_access_container("short_array: ",
                                short_array, 10);

  SingleArray<short> adapted_rvalue_array1 { std::move(short_array) };
  adapted_rvalue_array1[4] = 49;
  print_random_access_container("adapted_rvalue_array1: ",
                                adapted_rvalue_array1, 10);
  print_random_access_container("short_array: ",
                                short_array, 10);
  SingleArray<short> adapted_rvalue_array2 { adapted_rvalue_array1 };
  adapted_rvalue_array2[4] = 99;
  print_random_access_container("adapted_value array2: ",
                                adapted_rvalue_array2, 10);

  std::cout << "\nCopy assignment tests:\n";
  SingleArray<short> adapted_rvalue_array3 = adapted_rvalue_array2;
  adapted_rvalue_array2[9]=8888;
  print_random_access_container("adapted_value array3: ",
                                adapted_rvalue_array3, 10);

  SingleArray<short> adapted_short_array3 = adapted_short_array2;
  short_array[0]=777;
  print_random_access_container("adpated_short_array3 ",
                                adapted_short_array3, 10);

}

void MoveControlTest()
{
  std::cout << "\n************MoveControlTest****************\n";
  std::array<double, 8> double_array { 1.0, 2.3, 3.5, 4.6,
                                       5.7, 6.8, 7.9, 8.0};
  SingleArray<double>
  adapted_double_array1(double_array);
  adapted_double_array1[3] = adapted_double_array1[0]
                             +adapted_double_array1[2];
  SingleArray<double>
  adapted_double_array2(std::move(adapted_double_array1));

  /* adapted_double_array1[2]=10.0; Segmentation fault if removed */

  print_random_access_container("adpated_double_array2:",
                                adapted_double_array2, 8);

  print_random_access_container("double_array:",
                                double_array, 8);

  SingleArray<double> adapted_double_array3
    = std::move(adapted_double_array2);
  /* adapted_double_array2[4]=6.0; Segmentation fault if removed */
  print_random_access_container("adpated_double_array3:",
                                adapted_double_array3, 8);
  adapted_double_array2 = adapted_double_array3;
  print_random_access_container("adpated_double_array2:",
                                adapted_double_array2, 8);
}
//#endif
int main()
{
  LValueTest();
  RValueTest();
  CopyControlTest();
  MoveControlTest();
}