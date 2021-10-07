#include <iostream>
#include "functional-list.h"
#include "long20000list.h"
#include "longlist.h"


template<int ... intlist>
using IntList = typename Facility<int>::List<intlist...>;

template<char ... charlist>
using CharList = typename Facility<char>::List<charlist...>;

template<short ... shortlist>
using ShortList = typename Facility<short>::List<shortlist...>;

template<unsigned short ... shortlist>
using UnsignedShortList = typename Facility<unsigned short>::List<shortlist...>;

template<long ... list>
using LongList = typename Facility<long>::List<list...>;

void TestList()
{
    std::cout << "********Testing Basic List Capabilities*********\n";  
    using List1 = IntList<1,2,3,4>;
    using List2 = IntList<9, 0, -1, -200>;
   using List3 = IntList<LONG20000LIST>;
    List1::print();
    List2::print();
    List3::print();
    
    using String1 = CharList<'a', 'b', 'c'>;
    using String2 = CharList<' ', 'w', 'o', 'r', 'l', 'd' >;
    using EmptyString = CharList<>;
    String1::print();
    String2::print();
    EmptyString::print();
    std::cout << "************Finished Testing *******************\n\n";
}

void TestDotProduct()
{
    std::cout << "********Testing Dot Product Capabilities********\n";    
    
	using ShortFacility = Facility<short>; 	
	using FirstVec = ShortList<2, 8, 10>;
	using SecondVec = ShortList<-20, -5, 20>;
  //using FirstVec = ShortList<2>;
	//using SecondVec = ShortList<2>;
    std::cout << "First Vector is "; FirstVec::print();
    std::cout << "Second Vector is "; SecondVec::print();
    std::cout << "DotProduct is ";
	std::cout << ShortFacility::DotProduct<FirstVec, SecondVec>::result << std::endl;
    using EmptyVec = ShortList<>;
    std::cout << "Empty Vec is "; EmptyVec::print();
    std::cout << "DotProduct of EmptyVecs ";
	std::cout << ShortFacility::DotProduct<EmptyVec, EmptyVec>::result << std::endl;  
    
     // std::cout << ShortList::DotProduct<EmptyVec, FirstVec>::result << std::endl;      
      /*Should cause compile failure when uncommented.
      Cannot perform dot product of unequal sizes.
    */
    std::cout << "************Finished Testing *******************\n\n";
}

void TestConcat()
{
    std::cout << "*********** Testing Concatenation **************\n";    
    using UnsignedShortFac = Facility<unsigned short>;
    using EmptyList = UnsignedShortList<>;
    using List1 = UnsignedShortList<64438, 9000, 100, 2>;
    using List2 = UnsignedShortList<45, 454>;
    std::cout << "Concatenating ";
    List1::print();
    std::cout << " with ";
    List2::print();
    std::cout << " results in ";
    UnsignedShortFac::Concat<List1, List2>::result::print();
    std::cout << "Concatenating ";
    EmptyList::print();
    std::cout << " with ";
    List2::print();
    std::cout << " results in ";
    UnsignedShortFac::Concat<EmptyList, List2>::result::print();
    std::cout << "************Finished Testing *******************\n\n";
}

void TestRemoveFirst()
{
    std::cout << "*********** Testing RemoveFirst **************\n";    
    using IntFacility = Facility<int>;
using List = IntList<2, 8, 2, 3, 5, 10, 8, 5>;
IntFacility::RemoveFirst<2, List>::result::print(); 
/* prints "8 2 3 5 10 8 5" */
IntFacility::RemoveFirst<8, List>::result::print();
/* prints "2 2 3 5 10 8 5"*/
IntFacility::RemoveFirst<5, List>::result::print();
/* prints "2 8 2 3 10 8 5" */
IntFacility::RemoveFirst<9, List>::result::print();
/* prints "2 8 2 3 5 10 8 5" */
    std::cout << "************Finished Testing *******************\n\n";
}


void TestMin()
{
    using IntFacility = Facility<int>;
    using List1 = IntList<2, 8, 2, 3>;
    std::cout << IntFacility::Min<List1>::result << std::endl; /* prints 2 */
   /* 
	using EmptyList = IntList<>;
   	std::cout << Min<> << std::endl; */
   /* - compile error */ 
}

void TestPartition()
{
    std::cout << "*********** Testing Partition *******************\n";    
    using IntFac = Facility<>;
	/*
	// Should cause compile error if comments are removed
    std::cout << "Partitioning "; IntList<>::print();
    std::cout << "1st half: "; 
    IntFac::Partition< IntList<> >::FirstHalf::print();
    std::cout << "2nd half: "; 
    IntFac::Partition<IntList<> >::SecondHalf::print();
    */
    std::cout << "Partitioning "; IntList<5>::print();
    std::cout << "1st half: ";     
    IntFac::Partition<IntList<5> >::FirstHalf::print();
    std::cout << "2nd half: "; 
    IntFac::Partition<IntList<5> >::SecondHalf::print();

    using MyList = IntList<99, 2, 5, 100, 78, 120, 55, 129>;
    std::cout << "Partitioning "; MyList::print();     
    std::cout << "1st half: ";
    IntFac::Partition< MyList >::FirstHalf::print();
    std::cout << "2nd half: ";     
    IntFac::Partition< MyList >::SecondHalf::print(); 
    std::cout << "************Finished Testing *******************\n\n";
}

void TestSort()
{
    std::cout << "*********** Testing QuickSort *******************\n";    
	using LongFac = Facility<long>;
    using List = LongList<-2, -10, -5, -7, -5, -2, -4> ;
    std::cout << "Sorting this "; List::print();
	std::cout << "Result of sorting: "; 
    LongFac::Sort< List >::result::print();
    std::cout << "*********** Testing QuickSort *******************\n";    
}

void TestQuickSort()
{
    std::cout << "*********** Testing QuickSort *******************\n";    
	using LongFac = Facility<long>;
    using ShorterList = LongList<-2, -10, -5, -7, -5, -2, -4> ;
    using LongerList =  LongList<LONGLIST> ;
    std::cout << "Result of sorting: "; 
    LongFac::QuickSort< ShorterList >::result::print();
    std::cout << "Sorting this "; LongerList::print();
    std::cout << "Result of sorting: "; 
    LongFac::QuickSort<LongerList>::result::print();
    std::cout << "*********** Testing QuickSort *******************\n";    
}

int main()
{
    TestList();   
	TestDotProduct();
   TestConcat();
	TestMin();
	TestRemoveFirst();
	TestSort();
   TestPartition();
    TestQuickSort();
    return 0;
}