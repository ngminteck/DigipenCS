/******************************************************************************/
/*!
\file   HumanPlayer.h
\author Ng Min Teck
\par    email: minteck.ng\@digipen.edu
\par    DigiPen login: minteck.ng
\par    Course: CS225
\par    Assignment #2
\date   19/06/2018
\brief  
  *    Assignment 2 Type Erasure, SFINAE, Regex, mt19937, uniform distribution *
  *                                                                            *
  *    HumanPlayer Class contain:                                              * 
  *                              std::istream &stream_input                    *
  *                              bool print_input                              *
  *                              Constructor                                   *
  *                              PlayerMove function                           * 
  
  Hours spent on this assignment: 8

  Specific portions that gave you the most trouble: 
  The Board incude or class forward declare problem as it keep complain 
  incomplete type and i thought is my type erasure do something wrong. 
  etc/
  
*/
/******************************************************************************/
#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include <iostream> //std::cout , std::endl, std::cin
#include <regex> // std::regex, std::regex_match, std::smatch
#include <string> //std::string   std::stoi
#include <istream> //std::istream
#include <vector> //std::vector
#include "Board.h" // Board type and information
#include "PositionAndMove.h"// Move and Colour
#include <iterator>

/******************************************************************************/
/*!
  \class HumanPlayer
  \brief
   contain std::istream &stream_input and bool print_input declartion.
   constructor and PlayMove member function

*/
/******************************************************************************/
class HumanPlayer
{
    std::istream &stream_input; // for store the stream input
    bool print_input; // for print out the text or not
  public:
/******************************************************************************/
/*!
  \brief
   The constructor for HumanPlayer
   
  \param istream
    The stream input
    
  \param print
    To print out the text or not

*/
/******************************************************************************/
    HumanPlayer( std::istream & istream = std::cin, bool print= true);
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
    Move PlayMove(const Board& board , Colour c);
};

#endif
