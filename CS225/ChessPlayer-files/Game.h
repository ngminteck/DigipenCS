#ifndef GAME_H
#define GAME_H
#include <iostream>
#include "Player.h"
#include "Board.h"
class Game : public Board
{
public:
	Game(Player black, Player white, Colour start_colour = WHITE,
		std::istream& = std::cin);
	bool GameCompleted() const;
	Colour PlayGame(bool print = false);
private:
	Player black_player_;
	Player white_player_;
	Colour current_turn;
};
#endif
