
template<unsigned NumElements , typename ... Types> 
struct StructOfArray 
{
  void print(std:: ostream& ) const ; 
private :
  std ::tuple< std:: array<MemTypes , NumElements >... > arrays ;
};