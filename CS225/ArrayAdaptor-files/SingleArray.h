#include <utility> 
#include <type_traits>
#include <memory>

template <typename T>
class SingleArray 
{
  struct Concept
  {
    virtual T& operator[] (int index) = 0;
    virtual T operator[] (int index) const = 0;
    virtual ~Concept(){}
  };
  template<typename U>
  struct Model : Concept
  {
    template<typename V>
    Model(V&& t)
    :obj(std::forward<V>(t))
    {
       
    }
    
    T& operator[] (int index)
    {
      return obj[index];
    }
    
    T operator[] (int index) const
    {
      return obj[index];
    }

    U obj; 
  };
  
  std::shared_ptr<Concept> ptr;
  
  public:
    SingleArray()
    :ptr(nullptr)
    {
    
    }
    template<typename U,typename V = typename 
      std::enable_if_t<
        !std::is_same<
          std::decay_t<U>
          ,SingleArray>
        ::value>
      >
    SingleArray(U&& t)
    : ptr(new Model<U>(std::forward<U>(t)))
    {
      
    }
    
    SingleArray(const SingleArray& rhs)
    :ptr(rhs.ptr)
    {
    
    }
    
    SingleArray(SingleArray&& rhs):
    ptr(rhs.ptr)
    {
      rhs.ptr = nullptr;
    }
    
    SingleArray& operator=(const SingleArray& rhs)
    {
      ptr = rhs.ptr;
      return *this;
    }
    
    SingleArray& operator=(SingleArray&& rhs)
    {
      std::swap(ptr, rhs.ptr);
      return *this;
    }
    
    ~SingleArray()
    {
      
    }
    
    T operator[](int index)const
    {
      return ptr->operator[](index);
    }
    T& operator[](int index)
    {
       return ptr->operator[](index);
    }
};