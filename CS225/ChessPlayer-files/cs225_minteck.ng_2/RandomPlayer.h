/******************************************************************************/
/*!
\file   RandomPlayer.h
\author Ng Min Teck
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng
\par    Course: CS225
\par    Assignment #2
\date   19/06/2018
\brief  
  *    Assignment 2 Type Erasure, SFINAE, Regex, mt19937, uniform distribution *
  *                                                                            *
  *    RandomPlayer Class contain:                                             *
  *                              std::mt19937 gen                              *
  *                              Constructor                                   *
  *                              PlayerMove function                           *
  
  Hours spent on this assignment: 8

  Specific portions that gave you the most trouble: 
  The Board incude or class forward declare problem as it keep complain 
  incomplete type and i thought is my type erasure do something wrong. 
  etc/
  
*/
/******************************************************************************/
#ifndef RANDOMPLAYER_H
#define RANDOMPLAYER_H

#include <vector> //std::vector
#include <random> //std::uniform_int_distribution , std::mt19937
#include "Board.h" // contain all board information
#include "PositionAndMove.h" // Move and colour 

/******************************************************************************/
/*!
  \class RandomPlayer
  \brief
   contain std::mt19937 gen declartion. constructor and PlayMove member function
*/
/******************************************************************************/
class RandomPlayer
{ 
    std::mt19937 gen; // for init seed
  public:
/******************************************************************************/
/*!
  \brief
   The constructor for RandomPlayer
    
  \param seed
    The seed
*/
/******************************************************************************/
    RandomPlayer(unsigned seed);
/******************************************************************************/
/*!
  \brief
   To move the piece
   
  \param board
    The board information
    
  \param c
    The colour represent 
    
  \returns
     The Move the player make

*/
/******************************************************************************/
    Move PlayMove( const Board& board, Colour c );
};
#endif