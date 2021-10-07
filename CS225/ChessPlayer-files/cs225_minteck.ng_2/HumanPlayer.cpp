/******************************************************************************/
/*!
\file   HumanPlayer.cpp
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
#include "HumanPlayer.h" // humanplayer declaration 

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
HumanPlayer::HumanPlayer( std::istream & istream , bool print)
:stream_input{istream},print_input{print}
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
Move HumanPlayer::PlayMove(const Board& board, Colour c)
{
  std::vector<Move> legal_moves = board.GetAllLegalMoves(c);
  // legit command A-H and 0-7 form the chess
  // the lower case a-h will be auto convert to uppercase
  std::regex legit_command("\\(([A-Za-z]+)([0-9]+)-([A-Za-z]+)([0-9]+)\\)");
  while(true)
  {
    if(print_input)
      std::cout<<"Your turn to play:"<< std::endl;
    std::string input;
    stream_input >> input;

    while (!std::regex_match(input, legit_command))
    {
      if(print_input)
        std::cout<<"Invalid input. Please enter your move again."<< std::endl;
      stream_input >> input;
    }
    
    int start_col =0; 
    int start_row = 0; 
    int dest_col = 0; 
    int dest_row =0; 
    std::smatch input_format;
    std::string copy_col;
    
    if(regex_match(input, input_format, legit_command))
    {
      // copy the match input so that can modify later to int
      copy_col = input_format[1];
      // check is it upper or lower case.only want the first char of the string 
      if(copy_col[0] < 'a')
        start_col = copy_col[0] - 'A';
      else
        start_col = copy_col[0] - 'a';
     
      start_row = std::stoi(input_format[2]);
     
      copy_col = input_format[3];
      if(copy_col[0] < 'a')
        dest_col = copy_col[0] - 'A';
      else
        dest_col = copy_col[0] - 'a';
       
      dest_row = std::stoi(input_format[4]);
    }
    
    Position start_pos(start_col,start_row);
    Position dest_pos(dest_col,dest_row);
    
    Move move_command = {start_pos,dest_pos};
    
    for (auto& elem: legal_moves)
    {
      if(elem.orig_pos == move_command.orig_pos 
      && elem.dest_pos == move_command.dest_pos)
        return elem;
    }
    
    if(print_input)
      std::cout<<"Invalid move. Please enter another move.."<< std::endl;
  }
}