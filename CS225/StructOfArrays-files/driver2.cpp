#include <iostream>
#include <utility>
#include "reflection.h"
#include "gensoa.h"


class X {

  public:
    unsigned int f0;
    double f1;
    unsigned char f2;
    short f3;
    using MetaDataType = MetaData<X,unsigned int X::*,double X::*,unsigned char X::*,short X::* >;
    static MetaDataType metadata;
};
X::MetaDataType X::metadata {"X", std::make_tuple(&X::f0,&X::f1,&X::f2,&X::f3),std::make_tuple("f0","f1","f2","f3")};
template<size_t NumOfElements, typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, NumOfElements>& rhs)
{
  auto soa = GetSOA(rhs);
  soa.print(os);
  return os;
}


int main()
{
  std::array<X,9> v {{X{1580564856,0.078259,13,12812},X{1786219163,0.078259,95,-14546},X{539451695,0.078259,0,-19310},X{1441294128,0.078259,8,-14187},X{332584007,0.078259,123,8817},X{1257183443,0.078259,67,-31057},X{593474851,0.078259,84,-24239},X{121847932,0.078259,85,3768},X{1081466671,0.078259,15,-22857}}};

  std::cout << v << std::endl;
}
