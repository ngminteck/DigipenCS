#include <iostream>
#include "reflection.h"
#include "gensoa.h"
class Haha
{
	short x=78;
	float y=0.99f;
	std::string name ="cs225";
public:
     constexpr static MetaData<Haha, short Haha::*,  float Haha::*,
                              std::string Haha::*> metadata 
                              { "Haha", std::make_tuple(&Haha::x, &Haha::y, &Haha::name),
							    std::make_tuple("x", "y", "name")};    
	 
};

class Y
{
private:
    int haha;
    double who;
    std::string z; 
public:
    constexpr static MetaData<Y, int Y::*, double Y::*,
                              std::string Y::*> metadata 
                              { "Y", std::make_tuple(&Y::haha, &Y::who, &Y::z),
							    std::make_tuple("haha", "who", "z")};    
	Y(int h=10, double x=3.0, const std::string& s = "yeah")
       : haha{h}, who{x}, z{s}
	{
		
	}
};

template<size_t NumOfElements, typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, NumOfElements>& rhs)
{
	auto soa = GetSOA(rhs);
	soa.print(os); 
	return os; 
}

int main()
{
    std::array<Haha, 5> array_of_hahas;
	std::array<Y, 3> array_of_Ys { {Y{ 5, 6.0, "abc" }, Y{ 99, 2.4, "def" } }};  
	std::cout << array_of_hahas << array_of_Ys; 
}
