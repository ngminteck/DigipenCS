
#include "Game.h"

Game::Game(Player black, Player white,
	Colour start_colour, std::istream& is)
	: Board(is),
	black_player_(black),
	white_player_(white),
	current_turn(start_colour)
{

}

bool Game::GameCompleted() const
{
	int h = rows();
	int w = cols();
	bool white_king_found = false;
	bool black_king_found = false;
	for (int i = 0; i<h; ++i)
	{
		for (int j = 0; j<w; ++j)
		{
			auto state = GetState(Position(i, j));
			if (state == SquareState::WHITE_KING)
				white_king_found = true;
			if (state == SquareState::BLACK_KING)
				black_king_found = true;
		}
	}
	return !(white_king_found&&black_king_found);
}

Colour Game::PlayGame(bool print)
{
	while (!GameCompleted())
	{
		if (print)
		{
			std::cout << *this << std::endl;
			std::cout << std::endl;
		}
		Player& CurrentPlayer
			= (current_turn == BLACK) ? black_player_ : white_player_;
		auto m = CurrentPlayer.PlayMove(*this, current_turn);

		static_cast<Board*>(this)->MakeMove(m);
		current_turn = static_cast<Colour>(1 ^ static_cast<int>(current_turn));		
	}
	if (print)
	{
		std::cout << *this << std::endl;
		std::cout << std::endl;
	}

	
	return static_cast<Colour>(~current_turn);
}