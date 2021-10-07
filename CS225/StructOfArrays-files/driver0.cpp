#include <iostream>
#include <utility>
#include "reflection.h"
#include "gensoa.h"


class X {

    public:
        short f0;
        int f1;
        unsigned short f2;
        long f3;
        long f4;
        std::string f5;
        int f6;
        unsigned int f7;
        unsigned long f8;
        char f9;
        unsigned int f10;
        unsigned int f11;
        long f12;
        unsigned char f13;
        double f14;
        unsigned long f15;
        unsigned short f16;
        using MetaDataType = MetaData<X,short X::*,int X::*,unsigned short X::*,long X::*,long X::*,std::string X::*,int X::*,unsigned int X::*,unsigned long X::*,char X::*,unsigned int X::*,unsigned int X::*,long X::*,unsigned char X::*,double X::*,unsigned long X::*,unsigned short X::* >;
        static MetaDataType metadata;
};
X::MetaDataType X::metadata {"X", std::make_tuple(&X::f0,&X::f1,&X::f2,&X::f3,&X::f4,&X::f5,&X::f6,&X::f7,&X::f8,&X::f9,&X::f10,&X::f11,&X::f12,&X::f13,&X::f14,&X::f15,&X::f16),std::make_tuple("f0","f1","f2","f3","f4","f5","f6","f7","f8","f9","f10","f11","f12","f13","f14","f15","f16")};
template<size_t NumOfElements, typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, NumOfElements>& rhs)
{
    auto soa = GetSOA(rhs);
    soa.print(os);
    return os;
}


int main()
{
    std::array<X,0> v {{}};

    std::cout << v << std::endl;
}
