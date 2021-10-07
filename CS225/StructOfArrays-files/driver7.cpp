#include <iostream>
#include <utility>
#include "reflection.h"
#include "gensoa.h"


class X {

  public:
    std::string f0;
    unsigned int f1;
    int f2;
    double f3;
    float f4;
    short f5;
    int f6;
    float f7;
    long f8;
    unsigned short f9;
    using MetaDataType = MetaData<X,std::string X::*,unsigned int X::*,int X::*,double X::*,float X::*,short X::*,int X::*,float X::*,long X::*,unsigned short X::* >;
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
  std::array<X,12> v {{X{"vcigljMz5QsF4Pb",619599445,1,0.078259,0.078259,-11621,1,0.078259,1362765809,8406},X{"C50TnI8V",2083864808,1,0.078259,0.078259,7659,1,0.078259,167523702,14659},X{"POnRfZrpM4FdTBitaUj",741182423,1,0.078259,0.078259,-5478,1,0.078259,1355615178,8813},X{"Wfl0TH5FO1KmXGvZhMP",1817574772,1,0.078259,0.078259,-3916,1,0.078259,808130857,46028},X{"PdsjDw59ZN",1519287663,1,0.078259,0.078259,-26937,1,0.078259,1009886784,31728},X{"mIL5A3FdYaM2wHSf",2036814072,1,0.078259,0.078259,10066,1,0.078259,943478020,42341},X{"4qWJ7V",749488555,1,0.078259,0.078259,25341,1,0.078259,976266121,59721},X{"gGJ0zc",999604104,1,0.078259,0.078259,2181,1,0.078259,1958318899,58016},X{"nIFPKsRTxcOiZqCh1b",1973042251,1,0.078259,0.078259,5445,1,0.078259,548985481,63425},X{"nKraRfO",1894124163,1,0.078259,0.078259,-168,1,0.078259,129233770,48228},X{"3HUnT",175252714,1,0.078259,0.078259,-8230,1,0.078259,516200255,58845},X{"DTY1k40sc5R",366418567,1,0.078259,0.078259,9687,1,0.078259,2125660662,39941}}};

  std::cout << v << std::endl;
}
