#include <iostream>
#include <utility>
#include "reflection.h"
#include "gensoa.h"


class X {

  public:
    float f0;
    long f1;
    unsigned short f2;
    unsigned int f3;
    unsigned char f4;
    int f5;
    short f6;
    double f7;
    unsigned long f8;
    long f9;
    using MetaDataType = MetaData<X,float X::*,long X::*,unsigned short X::*,unsigned int X::*,unsigned char X::*,int X::*,short X::*,double X::*,unsigned long X::*,long X::* >;
    static MetaDataType metadata;
};
X::MetaDataType X::metadata {"X", std::make_tuple(&X::f0,&X::f1,&X::f2,&X::f3,&X::f4,&X::f5,&X::f6,&X::f7,&X::f8,&X::f9),std::make_tuple("f0","f1","f2","f3","f4","f5","f6","f7","f8","f9")};
template<size_t NumOfElements, typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, NumOfElements>& rhs)
{
  auto soa = GetSOA(rhs);
  soa.print(os);
  return os;
}


int main()
{
  std::array<X,2> v {{X{0.078259,1602103962,10727,1721771142,98,1,5408,0.078259,1698772085,1611883850},X{0.078259,2090187508,1095,1893364866,91,1,-24356,0.078259,27160264,1318734543}}};

  std::cout << v << std::endl;
}
