#ifndef REFLECTION_H_DEF
#define REFLECTION_H_DEF
#include <tuple>
#include <map>
#include <functional>
#include <array>
#include <memory>

template<unsigned N, typename ... Rest>
struct GenNameTupleType : GenNameTupleType<N-1, const char *, Rest...>
{

};

template<typename ... List>
struct GenNameTupleType<0, List...>
{
     using type = std::tuple<List...>;
};

template<typename T, typename ... PtrToMems>
class MetaData
{
    using NameTupleType 
       = typename GenNameTupleType<sizeof...(PtrToMems)>::type;
    using PtrMemTuple = std::tuple<PtrToMems...>;
    const char *class_name;    
	PtrMemTuple ptrs; 
    NameTupleType names;
    
    template<typename Container>
    using DataMap = std::map<std::string, Container>;

    template<typename TypeErasedObj, size_t ... Indices>
    auto reflect_aux(T& rhs, std::index_sequence<Indices...>) const
    {
        return DataMap<TypeErasedObj>
               {
                   std::make_pair(std::string(std::get<Indices>(names)), 
                                  TypeErasedObj{std::invoke(std::get<Indices>(ptrs), rhs)})...
               };
    }

public:
       
    template<typename TypeErasedObj>
    auto reflect(T& rhs) const
    {
        return reflect_aux<TypeErasedObj>(rhs, std::make_index_sequence<sizeof...(PtrToMems)>{});
    }
	
	const auto& GetPtrToMembers() const
	{
		return ptrs; 
	}

    constexpr MetaData(const char *cn, const PtrMemTuple&  ptr_tuple, 
                       const NameTupleType & name_tuple)
       : class_name{cn}, ptrs{ptr_tuple}, names{name_tuple}
    {

    }
};
#endif 
