#include <iostream>
#include <utility>
#include "reflection.h"
#include "gensoa.h"


class X {

  public:
    short f0;
    unsigned char f1;
    unsigned int f2;
    unsigned short f3;
    char f4;
    int f5;
    unsigned short f6;
    unsigned int f7;
    int f8;
    double f9;
    unsigned long f10;
    unsigned char f11;
    double f12;
    unsigned long f13;
    double f14;
    unsigned int f15;
    char f16;
    unsigned int f17;
    unsigned int f18;
    unsigned long f19;
    using MetaDataType = MetaData<X,short X::*,unsigned char X::*,unsigned int X::*,unsigned short X::*,char X::*,int X::*,unsigned short X::*,unsigned int X::*,int X::*,double X::*,unsigned long X::*,unsigned char X::*,double X::*,unsigned long X::*,double X::*,unsigned int X::*,char X::*,unsigned int X::*,unsigned int X::*,unsigned long X::* >;
    static MetaDataType metadata;
};
X::MetaDataType X::metadata {"X", std::make_tuple(&X::f0,&X::f1,&X::f2,&X::f3,&X::f4,&X::f5,&X::f6,&X::f7,&X::f8,&X::f9,&X::f10,&X::f11,&X::f12,&X::f13,&X::f14,&X::f15,&X::f16,&X::f17,&X::f18,&X::f19),std::make_tuple("f0","f1","f2","f3","f4","f5","f6","f7","f8","f9","f10","f11","f12","f13","f14","f15","f16","f17","f18","f19")};
template<size_t NumOfElements, typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, NumOfElements>& rhs)
{
  auto soa = GetSOA(rhs);
  soa.print(os);
  return os;
}


int main()
{
  std::array<X,6> v {{X{-8178,125,1325196885,45950,13,1,21258,541250951,1203257357,0.078259,1377301820,72,0.078259,707782292,0.078259,846482663,78,1186261522,1610459631,2084666298},X{-14896,15,1859775930,48516,115,1,61241,137106477,182621625,0.078259,20519809,6,0.078259,1267470542,0.078259,1140426118,12,1809921052,621407968,142919908},X{-22935,91,1175953858,27117,48,1,14112,638845045,851382209,0.078259,1699134019,67,0.078259,575289421,0.078259,792981699,33,1344189946,890267421,913212406},X{-2010,9,1273024128,2999,110,1,35510,878652573,456517332,0.078259,721059323,88,0.078259,316911880,0.078259,1531574716,49,690463907,552504006,2143519484},X{-27519,7,1221910763,18196,123,1,50500,371402518,400600880,0.078259,1737388254,4,0.078259,121479652,0.078259,875498735,117,1137741215,500195805,1574152676},X{-24543,11,182199024,56782,124,1,63698,259795473,975795125,0.078259,1119636543,41,0.078259,18601718,0.078259,361293847,33,858683241,1305189367,805910495}}};

  std::cout << v << std::endl;
}
