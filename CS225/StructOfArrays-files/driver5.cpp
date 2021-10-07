#include <iostream>
#include <utility>
#include "reflection.h"
#include "gensoa.h"


class X {

  public:
    unsigned char f0;
    char f1;
    int f2;
    unsigned int f3;
    unsigned short f4;
    double f5;
    double f6;
    unsigned short f7;
    unsigned short f8;
    using MetaDataType = MetaData<X,unsigned char X::*,char X::*,int X::*,unsigned int X::*,unsigned short X::*,double X::*,double X::*,unsigned short X::*,unsigned short X::* >;
    static MetaDataType metadata;
};
X::MetaDataType X::metadata {"X", std::make_tuple(&X::f0,&X::f1,&X::f2,&X::f3,&X::f4,&X::f5,&X::f6,&X::f7,&X::f8),std::make_tuple("f0","f1","f2","f3","f4","f5","f6","f7","f8")};
template<size_t NumOfElements, typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, NumOfElements>& rhs)
{
  auto soa = GetSOA(rhs);
  soa.print(os);
  return os;
}


int main()
{
  std::array<X,2> v {{X{93,121,1,52075049,45603,0.078259,0.078259,592,18453},X{47,127,1,659671834,13850,0.078259,0.078259,51342,2165}}};

  std::cout << v << std::endl;
}
