/******************************************************************************/
/*!
\file   RandomPlayer.cpp
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
#include "RandomPlayer.h" // the randomplayer declartion
/******************************************************************************/
/*!
  \brief
   The constructor for RandomPlayer
    
  \param seed
    The seed
*/
/******************************************************************************/
RandomPlayer::RandomPlayer(unsigned seed)
:gen{seed}
{
  
}
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
Move RandomPlayer::PlayMove ( const Board& board, Colour c )
{
   std::vector<Move> legal_moves = board.GetAllLegalMoves(c);
   
   std::uniform_int_distribution<int> dis(0,
   static_cast<int>(legal_moves.size()-1));

   return legal_moves[dis(gen)];
   
}