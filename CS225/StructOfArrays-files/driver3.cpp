#include <iostream>
#include <utility>
#include "reflection.h"
#include "gensoa.h"


class X {

  public:
    float f0;
    unsigned char f1;
    unsigned long f2;
    unsigned int f3;
    unsigned short f4;
    std::string f5;
    int f6;
    unsigned short f7;
    char f8;
    std::string f9;
    float f10;
    double f11;
    unsigned short f12;
    float f13;
    double f14;
    std::string f15;
    unsigned short f16;
    char f17;
    using MetaDataType = MetaData<X,float X::*,unsigned char X::*,unsigned long X::*,unsigned int X::*,unsigned short X::*,std::string X::*,int X::*,unsigned short X::*,char X::*,std::string X::*,float X::*,double X::*,unsigned short X::*,float X::*,double X::*,std::string X::*,unsigned short X::*,char X::* >;
    static MetaDataType metadata;
};
X::MetaDataType X::metadata {"X", std::make_tuple(&X::f0,&X::f1,&X::f2,&X::f3,&X::f4,&X::f5,&X::f6,&X::f7,&X::f8,&X::f9,&X::f10,&X::f11,&X::f12,&X::f13,&X::f14,&X::f15,&X::f16,&X::f17),std::make_tuple("f0","f1","f2","f3","f4","f5","f6","f7","f8","f9","f10","f11","f12","f13","f14","f15","f16","f17")};
template<size_t NumOfElements, typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, NumOfElements>& rhs)
{
  auto soa = GetSOA(rhs);
  soa.print(os);
  return os;
}


int main()
{
  std::array<X,12> v {{X{0.078259,54,1377438715,1950291021,9940,"05vyiIomgsfLVhTb",1,19855,27,"npyzGFVxZtMPefslD3",0.078259,0.078259,51942,0.078259,0.078259,"Wa50k",36439,59},X{0.078259,98,1006556100,679631323,46636,"GfHm6E",1,64971,37,"hGRuYgA",0.078259,0.078259,5154,0.078259,0.078259,"Da87kwJ6IMrGeKsdjRQ",28880,122},X{0.078259,6,655986852,561475344,2197,"W",1,58517,67,"h8fe6vI5US91",0.078259,0.078259,8055,0.078259,0.078259,"nVN6PX5bMRs",59810,89},X{0.078259,69,116119703,206983157,9646,"GJMLIHcUNtykd",1,46438,97,"hTDom4",0.078259,0.078259,16806,0.078259,0.078259,"u",41959,114},X{0.078259,95,1492694848,894728679,32287,"srDNPg0jfvmJbBZ",1,63175,76,"lX06oQdVaC",0.078259,0.078259,2147,0.078259,0.078259,"XpoaOG",51699,118},X{0.078259,52,1320293740,2008607267,64173,"Dym42iosqjNEnOx3e",1,16704,33,"",0.078259,0.078259,43081,0.078259,0.078259,"XvWi47qL2PMO3",15362,71},X{0.078259,52,1433239638,794078386,57418,"B2tNkSi6lRV",1,9696,41,"OCBfQA6cx",0.078259,0.078259,38739,0.078259,0.078259,"J8MBIOp6",62870,82},X{0.078259,127,1727743720,525510226,37040,"4HPjubfzkSRsxaie",1,9054,41,"5TJko8HZNcjKtPhS",0.078259,0.078259,22508,0.078259,0.078259,"4FP",30150,82},X{0.078259,79,1480468530,70829781,48729,"9MAV1",1,41223,113,"h2",0.078259,0.078259,57139,0.078259,0.078259,"Jc2s0FDV93aZUmk",43171,9},X{0.078259,124,2117442268,151187002,44779,"Ys",1,8704,36,"Bq7dlmEzTOyiuG",0.078259,0.078259,20957,0.078259,0.078259,"S0wKWp2",56863,21},X{0.078259,27,444359977,212465454,23365,"KSupX",1,56128,123,"Kb",0.078259,0.078259,46861,0.078259,0.078259,"IuinGLWYSfApcab7s",3679,116},X{0.078259,45,527199482,591765078,46136,"5uvd4JLMxkGiyoZ3sfA",1,15672,120,"1LN0awkpBIMTe",0.078259,0.078259,49981,0.078259,0.078259,"0Vk2UsJIfdzNiC",31603,119}}};

  std::cout << v << std::endl;
}
