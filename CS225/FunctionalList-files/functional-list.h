/******************************************************************************/
/*!
\file   functional-list.h
\author Ng Min Teck
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng
\par    Course: CS225
\par    Assignment #4
\date   21/07/2018
\brief  
  *    Assignment 4 AClass template, function template, template specialization*
  *    Meta-programming, Fold expression , if constexpr                        *
 
  
  Hours spent on this assignment: 72+

  Specific portions that gave you the most trouble: 
  
  The syntax of meta function,

  The implement of Partition, as i not really sure
  how to do, even my friend guide me along.
  
  The Pirnt function and later on i found there is fold expression to make 
  thing simple, but i don't have much time left to simplefiy the other code
  
  
  etc/
  
*/
/******************************************************************************/
#include <algorithm>
#include <vector>


template<typename T = int , typename Comp=std::less<T>>
struct Facility
{
  template<T ... list> 
  struct List
  {
    static void print()
    {
      // store in static array to compile faster
      std::vector<T> args = {list...};
      // use for idiom for each except the last, but this time in reserve way
      // assgn char* space to empty char first
      const char* space = "";
      if (sizeof...(list) == 0)
      {
        // simple just call space to cout , if not warning no variable used
        std::cout <<'"' <<"Empty List" <<'"' << space  <<std::endl;
      }
      else if  (std::is_same<T, char>::value)
      {
        std::cout << '"';
        for (auto const& elem : args)
        {
            std::cout << space << elem; 
        }
        std::cout << '"' << space<< std::endl;
      }
      else
      {
         std::cout << '"';
         for (auto const& elem : args)
         {
            std::cout << space << elem; 
            space = " ";
         }
         std::cout << '"' << std::endl;
      }    
    }  
  }; 
/******************************************************************************/
/*!
  \struct DotProduct
  \brief 
    Base class template of DotProduct.
  \tparam List1
    The first list
  \tparam List2
    The second list
*/
/******************************************************************************/
  template <typename List1, typename List2>
  struct DotProduct;
/******************************************************************************/
/*!
  \struct DotProduct
  \brief 
    Specialization of DotProduct when is more than 1 element
  \tparam Head1
    The first head
  \tparam Head2
    The second head
  \tparam Tails1
    The first tail
  \tparam Tails2
    The second tail
*/
/******************************************************************************/
  template <T Head1, T Head2, T... Tails1, T... Tails2>
  struct DotProduct< List<Head1, Tails1...>, List<Head2, Tails2...> >
  {
    constexpr static T result = 
    Head1 * Head2 + DotProduct<List<Tails1...>, List<Tails2...>>::result;
  };
/******************************************************************************/
/*!
  \struct DotProduct
  \brief 
    Specialization of DotProduct when is only left the head
  \tparam Head1
    The first head
  \tparam Head2
    The second head
*/
/******************************************************************************/
  template <T Head1, T Head2>
  struct DotProduct< List<Head1>, List<Head2>>
  {
    constexpr static T result = Head1 * Head2;
  };
/******************************************************************************/
/*!
  \struct DotProduct
  \brief 
    Specialization of DotProduct to check if is equal
  \tparam Head1
    The first head
  \tparam Head2
    The second head
*/
/******************************************************************************/
  template <T... Head1, T... Head2>
  struct DotProduct< List<Head1...>, List<Head2...>>
  {
    constexpr static T result = T();
    using CheckIfSameSize = 
      typename std::enable_if<sizeof...(Head1) == sizeof...(Head2)>::type;
  };
/******************************************************************************/
/*!
  \struct Concat
  \brief 
    Base class template of Concat.
  \tparam List1
    The first list
  \tparam List2
    The second list
*/
/******************************************************************************/
  template <typename List1, typename List2>
  struct Concat;
/******************************************************************************/
/*!
  \struct Concat
  \brief 
    Specialization of Concat, to concat 2 list
  \tparam ListValue1
    The first list value
  \tparam ListValue2
    The second list value
*/
/******************************************************************************/
  template <T... ListValue1, T... ListValue2>
  struct Concat< List<ListValue1...>, List<ListValue2...> >
  {
    using result = List<ListValue1..., ListValue2...>;
  };
/******************************************************************************/
/*!
  \struct RemoveFirst
  \brief 
    Base class template of RemoveFirst.
  \tparam Num
    The num wnat to find
  \tparam TheList
    The list
*/
/******************************************************************************/
  template <T Num, typename TheList>
  struct RemoveFirst;
/******************************************************************************/
/*!
  \struct RemoveFirst
  \brief 
    Specialization of RemoveFirst, when the num is not equal the head
  \tparam Num
    The num wnat to find
  \tparam Head
    The head
  \tparam Tails
    The Tails
*/
/******************************************************************************/
  template <T Num, T Head, T... Tails>
  struct RemoveFirst<Num, List<Head, Tails...> >
  {
    using result = typename Concat<List<Head>, 
                    typename RemoveFirst<Num, List<Tails...> >::result
                    >::result;
  };
/******************************************************************************/
/*!
  \struct RemoveFirst
  \brief 
    Specialization of RemoveFirst, when the num is equal with the head
  \tparam Num
    The num wnat to find
  \tparam Tails
    The Tails
*/
/******************************************************************************/
  template <T Num, T... Tails>
  struct RemoveFirst<Num, List<Num, Tails...> >
  {
    using result = List<Tails...>;
  };
/******************************************************************************/
/*!
  \struct RemoveFirst
  \brief 
    Specialization of RemoveFirst, when the num is cannot be found in the list
  \tparam Num
    The num want to find
  \tparam Head
    The Head
*/
/******************************************************************************/
  template <T Num, T Head>
  struct RemoveFirst<Num, List<Head>>
  {
    using result = List<Head>;
  };
/******************************************************************************/
/*!
  \struct RemoveFirst
  \brief 
    Specialization of RemoveFirst, when only 1 item in list and begin remove
  \tparam Num
    The num want to find
*/
/******************************************************************************/
  template <T Num>
  struct RemoveFirst<Num, List<Num>>
  {
    using result = List<>;
  };
/******************************************************************************/
/*!
  \struct Min
  \brief 
    Base class template of Min.
  \tparam TheList
    The list
*/
/******************************************************************************/
  template <typename TheList>
  struct Min;
/******************************************************************************/
/*!
  \struct Min
  \brief 
    Specialization of Min, when there is more list value to compare
  \tparam Smallest
    The Smallest value currently
  \tparam Head
    The Head
  \tparam Tails
    The Tails
*/
/******************************************************************************/
  template <T Smallest, T Head, T... Tails>
  struct Min< List<Smallest, Head, Tails...> >
  {
    constexpr static T result = Comp{}(Smallest, Head) ? 
    Min< List<Smallest, Tails...> >::result :
    Min< List<Head, Tails...> >::result;
  };
/******************************************************************************/
/*!
  \struct Min
  \brief 
    Specialization of Min, when the value found is smallest than current 
    smallest value
  \tparam Smallest
    The Smallest value currently
*/
/******************************************************************************/
  template <T Smallest>
  struct Min< List<Smallest> >
  {
    constexpr static T result = Smallest;
  };
/******************************************************************************/
/*!
  \struct Sort
  \brief 
    Base class template of Sort.
  \tparam TheList
    The list
*/
/******************************************************************************/
  template <typename TheList>
  struct Sort;
/******************************************************************************/
/*!
  \struct Sort
  \brief 
    Specialization of Sort, when still unsorted
  \tparam Tails
    The tails value
*/
/******************************************************************************/
  template <T...Tails>
  struct Sort< List<Tails...> >
  {
    constexpr static T curr_min = Min< List<Tails...> >::result;
    using unsortedList = typename RemoveFirst<curr_min,List<Tails...> >::result;
    using recursiveSortList = typename Sort<unsortedList>::result;
    using result = typename Concat< List<curr_min>, recursiveSortList>::result;
  };
/******************************************************************************/
/*!
  \struct Sort
  \brief 
    Specialization of Sort, when only left 1 value
  \tparam ListValue
    The ListValue
*/
/******************************************************************************/
  template <T ListValue>
  struct Sort< List<ListValue> >
  {
    using result = List<ListValue>;
  };
/******************************************************************************/
/*!
  \struct Partition
  \brief 
    Base class template of Partition.
  \tparam TheList
    The list
*/
/******************************************************************************/
  template <typename TheList>
  struct Partition;
/******************************************************************************/
/*!
  \struct Partition
  \brief 
    Base class template of Partition_Impl.
  \tparam SmallValue
    The SmallValue
  \tparam BigValue
    The BigValue
  \tparam UncheckValue
    The UncheckValue
*/
/******************************************************************************/
  template <typename SmallValue, typename LargeValue, typename UncheckValue>
  struct Partition_Impl;
/******************************************************************************/
/*!
  \struct DummyPartition
  \brief
     Base class template of DummyPartition.
  \tparam SmallValue
    The SmallValue
  \tparam LargeValue
    The LargeValue
*/
/******************************************************************************/
  template <typename SmallValue, typename LargeValue>
  struct DummyPartition
  {
    using FirstPartition = SmallValue;
    using SecondPartition = LargeValue;
  };
/******************************************************************************/
/*!
  \struct Partition
  \brief 
    Specialization of Partition.
  \tparam Tails
    The list value
*/
/******************************************************************************/
  template <T... Tails>
  struct Partition< List<Tails...> >
  {
    using Implementation = 
    typename Partition_Impl< List<>, List<>, List<Tails...> >::result;
    using FirstHalf = typename Implementation::FirstPartition;
    using SecondHalf = typename Implementation::SecondPartition;
  };
/******************************************************************************/
/*!
  \struct Partition_Impl
  \brief 
    Specialization of Partition_Impl. sort the remainder smaller or larger
  \tparam Pivot
    The Pivot
  \tparam Head
    The Head
  \tparam Tails1
    The Tails1
  \tparam Tails2
    The Tails2
  \tparam remainder
    The remainder
*/
/******************************************************************************/
  template <T Pivot, T Head, T... Tails1, T... Tails2, T... remainder>
  struct Partition_Impl< List<Tails1...>, List<Tails2...>, 
                         List<Pivot, Head, remainder...> >
  {
    using SmallerThanPivot = 
    Partition_Impl< List<Tails1..., Head>,List<Tails2...>,
    List<Pivot, remainder...> >;  

    using LargerThanPivot = 
    Partition_Impl< List<Tails1...>, List<Tails2..., Head>, 
    List<Pivot, remainder...> >;
    
    using result =
    typename std::conditional_t<Comp{}(Head, Pivot), SmallerThanPivot,
    LargerThanPivot>::result;                        
  };
/******************************************************************************/
/*!
  \struct Partition_Impl
  \brief 
    Specialization of Partition_Impl. when all been checked
  \tparam Pivot
    The Pivot
  \tparam Tails1
    The Tails1
  \tparam Tails2
    The Tails2
*/
/******************************************************************************/
  template <T Pivot, T... Tails1, T... Tails2>
  struct Partition_Impl< List<Tails1...>, List<Tails2...>, List<Pivot> >
  {
    using result = DummyPartition< List<Tails1...>, List<Tails2...>>;
  };
/******************************************************************************/
/*!
  \struct QuickSort
  \brief 
    Base of QuickSort
  \tparam TheList
    The TheList
  \tparam Dummy
    The Dummy
*/
/******************************************************************************/
  template <typename TheList, typename Dummy = void>
  struct QuickSort;
/******************************************************************************/
/*!
  \struct QuickSort
  \brief 
    Specialization of QuickSort, using parition to do quick sort
  \tparam Head
    The Head
  \tparam Tails
    The Tails
*/
/******************************************************************************/
  template <T Head, T... Tails>
  struct QuickSort< List<Head, Tails...> >
  {
    using FirstPart = typename Partition<List<Head, Tails...> >::FirstHalf;
    using SecondPart = typename Partition<List<Head, Tails...> >::SecondHalf;
    using List_1 = typename QuickSort<FirstPart>::result;
    using List_2 = typename QuickSort<SecondPart>::result;
    using List1_and_Pivot = typename Concat<List_1, List<Head>>::result;
    using result = typename Concat<List1_and_Pivot, List_2>::result;
  };
/******************************************************************************/
/*!
  \struct QuickSort
  \brief 
    Specialization of QuickSort, when no elem left
  \tparam Dummy
    The Dummy
*/
/******************************************************************************/
  template <typename Dummy>
  struct QuickSort< List<>, Dummy>
  {
    using result = List<>;
  };

};


 
 