#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <array>
#include <complex>
#include <iostream>
#include <fstream>
#include <set>
#include "Player.h"
#include "Piece.h"



const int NUM_PLAYERS = 2;
const int DEFAULT_ROWS = 8;
const int DEFAULT_COLS = 8;
enum PieceType
{
	EMPTY = 0,
	PAWN = 1,
	KNIGHT = 2,
	BISHOP = 3,
	ROOK = 4,
	QUEEN = 5,
	KING = 6
};

enum class SquareState
{
	EMPTY = PieceType::EMPTY,
	WHITE_PAWN = PieceType::PAWN,
	WHITE_KNIGHT = PieceType::KNIGHT,
	WHITE_BISHOP = PieceType::BISHOP,
	WHITE_ROOK = PieceType::ROOK,
	WHITE_QUEEN = PieceType::QUEEN,
	WHITE_KING = PieceType::KING,

	BLACK_PIECE = 8,
	BLACK_PAWN = PieceType::PAWN + BLACK_PIECE,
	BLACK_KNIGHT = PieceType::KNIGHT + BLACK_PIECE,
	BLACK_BISHOP = PieceType::BISHOP + BLACK_PIECE,
	BLACK_ROOK = PieceType::ROOK + BLACK_PIECE,
	BLACK_QUEEN = PieceType::QUEEN + BLACK_PIECE,
	BLACK_KING = PieceType::KING + BLACK_PIECE,
	ILLEGAL = 0xF,

};

bool IsBlack(SquareState s);
bool IsWhite(SquareState s);
bool IsEmpty(SquareState s);
bool IsIllegal(SquareState s);

const int EXTRA_ROWS = 4;
const int EXTRA_COLS = 4;
const int EXTRA_COLS_AT_FRONT = 2;
const int EXTRA_ROWS_ON_TOP = 2;

class Board
{
public:             
    Board(std::istream& is);
	Board(const Board&);
    SquareState GetState(const Position& ) const;
    std::vector<Move> GetAllLegalMoves(Colour col) const;	
	void MakeMove(const Move &);	
	int rows() const { return rows_; }
	int cols() const { return cols_; }
	virtual ~Board();
private:
	using PiecePtr = Piece *;
	using PiecePtrContainer = std::vector<PiecePtr> ;
	int ConvertPositionIntoIndex(const Position &pos) const 
	{
		return  (EXTRA_COLS+cols_)*(EXTRA_ROWS_ON_TOP + pos.row())
			+ pos.col() + EXTRA_COLS_AT_FRONT;
	}
	SquareState& GetState(const Position&);
	PiecePtr & GetPiecePtr(const Position& );
    int rows_, cols_;
    std::vector<SquareState> states_;
	std::vector<PiecePtr> all_pieces_;
	PiecePtrContainer white_pieces_;
	PiecePtrContainer black_pieces_;
};

std::ostream& operator<<(std::ostream& , const SquareState& );
std::ostream& operator<<(std::ostream& , const Board&);
#endif
