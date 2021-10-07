#include <iostream>
#include <utility>
#include "reflection.h"
#include "gensoa.h"


class X {

  public:
    unsigned short f0;
    std::string f1;
    char f2;
    unsigned int f3;
    std::string f4;
    unsigned int f5;
    float f6;
    double f7;
    unsigned short f8;
    unsigned int f9;
    unsigned short f10;
    unsigned char f11;
    using MetaDataType = MetaData<X,unsigned short X::*,std::string X::*,char X::*,unsigned int X::*,std::string X::*,unsigned int X::*,float X::*,double X::*,unsigned short X::*,unsigned int X::*,unsigned short X::*,unsigned char X::* >;
    static MetaDataType metadata;
};
X::MetaDataType X::metadata {"X", std::make_tuple(&X::f0,&X::f1,&X::f2,&X::f3,&X::f4,&X::f5,&X::f6,&X::f7,&X::f8,&X::f9,&X::f10,&X::f11),std::make_tuple("f0","f1","f2","f3","f4","f5","f6","f7","f8","f9","f10","f11")};
template<size_t NumOfElements, typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, NumOfElements>& rhs)
{
  auto soa = GetSOA(rhs);
  soa.print(os);
  return os;
}


int main()
{
  std::array<X,1> v {{X{34068,"bsCQkXPWhIy",7,544239316,"6mlvFuD93RCH2tOw",1695890113,0.078259,0.078259,3382,1320507007,50909,51}}};

  std::cout << v << std::endl;
}
