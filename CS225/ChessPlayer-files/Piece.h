#ifndef PIECE_H
#define PIECE_H
#include <vector>
#include "PositionAndMove.h"
class Board; 

enum class PieceChar : char
{
	WHITE_PAWN = 'P',
	WHITE_KNIGHT = 'N',
	WHITE_BISHOP = 'B',
	WHITE_ROOK = 'R',
	WHITE_QUEEN = 'Q',
	WHITE_KING = 'K',

	BLACK_PAWN = 'p',
	BLACK_KNIGHT = 'n',
	BLACK_BISHOP = 'b',
	BLACK_ROOK = 'r',
	BLACK_QUEEN = 'q',
	BLACK_KING = 'k',
	EMPTY = 'e',
};

class Piece
{
public: 
	using Direction = Position;
	Piece(Colour c, const Position& pos)
		: colour_(c), pos_(pos)
	{

	}

	virtual void GenMoves(const Board &, std::vector<Move>& ) const = 0;
	Colour colour() const { return colour_; }
	void position(const Position& pos) { pos_ = pos; }
	const Position& position() const { return pos_;  }
	virtual Piece *Create(const Position& pos) const = 0;
	virtual Piece *Clone() const = 0;
	static Piece* Factory(PieceChar, const Position& pos);
	virtual ~Piece() {}
private:
    Colour colour_;
	Position pos_;
};

struct PieceComp
{
	bool operator()(Piece *lhs, Piece * rhs) const
	{
		auto lhs_pos = lhs->position();
		auto rhs_pos = rhs->position();
		return (lhs_pos.col() < rhs_pos.col()) 
			    || (lhs_pos.col() == rhs_pos.col() && lhs_pos.row() < rhs_pos.row());
		
	}
};

const Piece::Direction BLACK_PAWN_DIR(0,-1);
const Piece::Direction WHITE_PAWN_DIR(0,1);
const Piece::Direction NEXT_COL(1, 0);
const Piece::Direction PREV_COL(-1, 0);

class Pawn : public Piece
{
public:
	Pawn(Colour, const Position&, bool=true);	
	void GenMoves(const Board &, std::vector<Move>&) const;
	virtual Pawn *Create(const Position& pos) const;
	virtual Pawn *Clone() const;
private:
	using Direction = Position;
	Direction dir_;
	bool done_first_move;
};

class Knight : public Piece
{
public:
	Knight(Colour, const Position&);
	void GenMoves(const Board &, std::vector<Move>&) const;
	virtual Knight *Create(const Position& pos) const;
	virtual Knight *Clone() const;
};

class Bishop : virtual public Piece
{
public:
	Bishop(Colour, const Position&);
	void GenMoves(const Board &, std::vector<Move>&) const;
	virtual Bishop *Create(const Position& pos) const;
	virtual Bishop *Clone() const;
};

class Rook : virtual public Piece
{
public:
	Rook(Colour, const Position&);
	void GenMoves(const Board &, std::vector<Move>&) const;
	virtual Rook *Create(const Position& pos) const;
	virtual Rook *Clone() const;
};

class King : public Piece
{
public:
	King(Colour, const Position&);
	void GenMoves(const Board &, std::vector<Move>&) const;
	virtual King *Create(const Position& pos) const;
	virtual King *Clone() const;
};

class Queen : virtual Piece, public Bishop, public Rook
{
public:
	Queen(Colour, const Position&);
	void GenMoves(const Board &, std::vector<Move>&) const;
	virtual Queen *Create(const Position& pos) const;
	virtual Queen *Clone() const;
};

#endif
