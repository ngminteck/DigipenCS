/******************************************************************************/
/*!
\file   Player.h
\author Ng Min Teck
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng
\par    Course: CS225
\par    Assignment #2
\date   19/06/2018
\brief  
  *    Assignment 2 Type Erasure, SFINAE, Regex, mt19937, uniform distribution *
  *                                                                            *
  *    Player Class contain:                                                   *
  *                         Struct Concept                                     *
  *                         Struct Model                                       *
  *                         Concept ptr                                        *
  *                         Default constructor                                *
  *                         Forwarding constructor                             *
  *                         Copy constructor                                   *
  *                         Move constructor                                   *
  *                         Copy assignment                                    *
  *                         Move assignment                                    *
  *                         Destructor                                         *
  *                         PlayerMove function                                * 
  
  Hours spent on this assignment: 8

  Specific portions that gave you the most trouble: 
  The Board incude or class forward declare problem as it keep complain 
  incomplete type and i thought is my type erasure do something wrong. 
  etc/
  
*/
/******************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H
#include <utility> //  std::forward
#include <type_traits>// std::enable_if, std::is_same, std::decay
#include <algorithm> // std::swap
#include "PositionAndMove.h"// for move and colour type

class Board;// Board class forward declare
/******************************************************************************/
/*!
  \class Player
  \brief
   Player Class, contain struct concept, struct model, conecpt ptr,no arugment 
   constructor, forward constructor, copy constructor, move constructor,
   copy assignment, move assignment, destructor and playermove member function
*/
/******************************************************************************/
class Player
{
/******************************************************************************/
/*!
  \struct Concept
  \brief
   struct concept, contain pure virtual playmove, pure virtual clone const
   and virtual destructor.
*/
/******************************************************************************/
  struct Concept
  {
/******************************************************************************/
/*!
  \brief
   Pure virtual PlayMove

  \param board
    The information of the board
    
  \param c
    The colour represent
*/
/******************************************************************************/
    virtual Move PlayMove(const Board& board, Colour c)=0;
/******************************************************************************/
/*!
  \brief
   Pure virtual clone const
*/
/******************************************************************************/
    virtual Concept* clone() const = 0;
/******************************************************************************/
/*!
  \brief
   virtual destrutor
*/
/******************************************************************************/
    virtual ~Concept(){}
  };
/******************************************************************************/
/*!
  \struct Model
  \brief
   Templated struct model, inherit from concept. Contain forward contstructor,
   PlayMove member function and clone fucntion

*/
/******************************************************************************/
  template<typename T>
  struct Model : Concept
  {
/******************************************************************************/
/*!
  \brief
   Forward constructor, that take in rvalue. 

  \param t
    The rhs value that pass through from player class forward constructor
*/
/******************************************************************************/
    template<typename U>
    Model(U&& t)
    :obj(std::forward<U>(t))
    {
       
    }
/******************************************************************************/
/*!
  \brief
   The function that want to link with unrelated class or struct using type
   erasure, it link with HumanPlayer class and RandomPlayer class and it will
   call obj playmove function
   
  \param board
    The information of the board
    
  \param c
    The colour represent
    
  \returns
     The Move returned from the obj function
*/
/******************************************************************************/
    Move PlayMove(const Board& board, Colour c)
    {
      return obj.PlayMove(board, c);
    }
/******************************************************************************/
/*!
  \brief
    A function to clone a Model use in player copy constructor 
  \returns
    A pointer the cloned Model
*/
/******************************************************************************/
    Model<T>* clone() const
    { 
       return new Model(*this);
    }
    T obj; // the T obj
  };
  Concept *ptr; //The pointer to the obj Model/Concept
  public:
/******************************************************************************/
/*!
  \brief
    Default constructor of player, initializes concept pointer to nullptr.
*/
/******************************************************************************/
  Player()
  :ptr {nullptr}
  {
    
  }
/******************************************************************************/
/*!
  \brief
    Forwarding constructor that takes in a player type
    Template instantiation is rejected if T is a derived class of 
    Player or same as Player 
  \param t
    A universal reference to an object of type T to construct
*/
/******************************************************************************/
  template<typename T,typename U = typename 
  std::enable_if<
    !std::is_same<
      typename std::decay<T>
      ::type,Player>
    ::value >
  ::type>
  Player(T&& t)
  : ptr(new Model<T>(std::forward<T>(t)))
  {
      
  }
/******************************************************************************/
/*!
  \brief
    Copy constructor for the player class
  \param rhs
    Const reference to a player to copy
*/
/******************************************************************************/
  Player(const Player& rhs)
  : ptr((rhs.ptr == nullptr) ? nullptr : rhs.ptr->clone())
  {
      
  }
/******************************************************************************/
/*!
  \brief
    Move constructor for the player class 
  \param rhs
    Rvalue reference to a player to move
*/
/******************************************************************************/
  Player(Player&& rhs)
  : ptr(rhs.ptr)
  {
    rhs.ptr = nullptr;
  }
/******************************************************************************/
/*!
  \brief
    Copy assignment for the player class
  \param rhs
    Const reference to a player to copy assign on it
  \returns
    A this ptr
*/
/******************************************************************************/
  Player& operator=(const Player& rhs)
  {
    if (this != &rhs)
    {
      Concept* tmp = rhs.ptr->clone();
      delete ptr;
      ptr = tmp;
    }
    return *this;
  }
/******************************************************************************/
/*!
  \brief
    Move assignment for the player class
  \param rhs
    Rvalue reference to a player to swap
  \returns
   A this ptr
*/
/******************************************************************************/
  Player& operator=(Player&& rhs)
  {
    std::swap(ptr, rhs.ptr);
    return *this;
  }
/******************************************************************************/
/*!
  \brief
    Destructor for the player class.
*/
/******************************************************************************/
  ~Player()
  {
    delete ptr;
  }
/******************************************************************************/
/*!
  \brief
    Calls obj Model PlayMove function.
  \param board
    The information to the board 
  \param c
    The colour represent
  \returns
    The Move returned from the obj Model PlayMove function.
*/
/******************************************************************************/
  Move PlayMove(const Board& board, Colour c)
  {
     return ptr->PlayMove(board, c);
  }
};
#endif