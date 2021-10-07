#include <iostream>
#include <utility>
#include "reflection.h"
#include "gensoa.h"


class X {

  public:
    char f0;
    int f1;
    long f2;
    int f3;
    unsigned short f4;
    float f5;
    unsigned int f6;
    using MetaDataType = MetaData<X,char X::*,int X::*,long X::*,int X::*,unsigned short X::*,float X::*,unsigned int X::* >;
    static MetaDataType metadata;
};
X::MetaDataType X::metadata {"X", std::make_tuple(&X::f0,&X::f1,&X::f2,&X::f3,&X::f4,&X::f5,&X::f6),std::make_tuple("f0","f1","f2","f3","f4","f5","f6")};
template<size_t NumOfElements, typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, NumOfElements>& rhs)
{
  auto soa = GetSOA(rhs);
  soa.print(os);
  return os;
}


int main()
{
  std::array<X,3> v {{X{32,353816843,1281515525,1,36139,0.078259,363392895},X{52,1903260693,2102832907,1,8625,0.078259,376991832},X{27,745107470,1626541090,1,45877,0.078259,570306641}}};

  std::cout << v << std::endl;
}
