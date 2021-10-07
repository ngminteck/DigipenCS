#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <random>
#include "Game.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "RandomPlayer.h"


struct GameConfig
{
    std::string filename;
    Player p1;
    Player p2;
};

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))
struct Parser
{
	int argc;
	char **argv;
	GameConfig gc;

	void parse()
	{
		int options = argc >= EXPECTED_NUM_ARGS;
		using PARSE_MEM_FN = void (Parser::*)();
		PARSE_MEM_FN parse_fn[NUM_PARSE_FUNCTIONS]
		     { &Parser::parse_error, &Parser::parse_correct };
		CALL_MEMBER_FN(*this, parse_fn[options]) ();
	}

private:
	static constexpr int PLAYER1_RANDOM = 0x1;
	static constexpr int PLAYER2_RANDOM = 0x2;
	static constexpr int NUM_PARSE_FUNCTIONS = 2;
	static constexpr int EXPECTED_NUM_ARGS = 3;
	static constexpr int EXECUTABLE_NAME = 0;
	static constexpr int FILENAME_ARG = 2;
	static constexpr int CHOICE_ARG = 1;
	static constexpr int SEED_ARG = 3;
    void parse_error()
    {
		std::cerr << "Usage: " << argv[EXECUTABLE_NAME];
		std::cerr << " <PlayerConfig:int> <BoardStateFileName:string> <RandomSeed:int>\n";
		std::cerr << "       Player Config Choice:\n";
		std::cerr << "       1. Human-Human \n";
		std::cerr << "       2. Human-Random \n";
		std::cerr << "       3. Random-Human \n";
		std::cerr << "       4. Random-Random \n";
		std::cerr << "       Any other choice is undefined \n";
		std::cerr << "       RandomSeed is an optional argument. Default will be 1.\n";
		exit(1);
    }

    void parse_correct()
    {
		gc.filename = std::string(argv[FILENAME_ARG]);
		int choice = std::atoi(argv[CHOICE_ARG])-1;
		int seed = (argc > SEED_ARG) ? (std::atoi(argv[3])) : 1;
		gc.p1 = ((choice) & PLAYER1_RANDOM) ? Player(RandomPlayer(seed)) : Player(HumanPlayer());
		gc.p2 = ((choice) & PLAYER2_RANDOM) ? Player(RandomPlayer(seed)) : Player(HumanPlayer());
    }
};

	int main(int argc, char **argv)
	{
		Parser p { argc, argv,  GameConfig() };
		p.parse();
		std::ifstream fs(p.gc.filename);
		Game g(p.gc.p1, p.gc.p2, WHITE, fs);
		g.PlayGame(true);
		return 0;
	}
