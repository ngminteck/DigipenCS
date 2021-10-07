#include "PositionAndMove.h"
#include "Piece.h"
#include"Board.h"
#include <map>
#include <utility>
#include <memory>

Pawn::Pawn(Colour c, const Position& pos, bool dfm)
  : Piece(c, pos),
    dir_(c==BLACK?BLACK_PAWN_DIR:WHITE_PAWN_DIR),
    done_first_move(dfm)
{


}

Piece* Piece::Factory(PieceChar pc, const Position &pos)
{
  using PiecePtr = std::shared_ptr < Piece > ;
  static std::map<PieceChar, PiecePtr> PieceCharToPiecePtr
  {
    { PieceChar::WHITE_PAWN, PiecePtr(new Pawn(WHITE, Position())) },
    { PieceChar::BLACK_PAWN, PiecePtr(new Pawn(BLACK, Position())) },
    { PieceChar::WHITE_KNIGHT, PiecePtr(new Knight(WHITE, Position())) },
    { PieceChar::BLACK_KNIGHT, PiecePtr(new Knight(BLACK, Position())) },
    { PieceChar::WHITE_ROOK, PiecePtr(new Rook(WHITE, Position())) },
    { PieceChar::BLACK_ROOK, PiecePtr(new Rook(BLACK, Position())) },
    { PieceChar::WHITE_QUEEN, PiecePtr(new Queen(WHITE, Position())) },
    { PieceChar::BLACK_QUEEN, PiecePtr(new Queen(BLACK, Position())) },
    { PieceChar::WHITE_KING, PiecePtr(new King(WHITE, Position())) },
    { PieceChar::BLACK_KING, PiecePtr(new King(BLACK, Position())) },
    { PieceChar::WHITE_BISHOP, PiecePtr(new Bishop(WHITE, Position())) },
    { PieceChar::BLACK_BISHOP, PiecePtr(new Bishop(BLACK, Position())) },
  };
  //return nullptr;
  return PieceCharToPiecePtr[pc]->Create(pos);
}

void Pawn::GenMoves(const Board& board, std::vector<Move>& moves) const
{
  std::vector<Position> non_capture_moves { dir_ };
  if (!done_first_move) non_capture_moves.push_back(dir_ + dir_);
  Position capture_moves[2] = { dir_ + NEXT_COL, dir_ + PREV_COL };
  auto IsOppColour = (colour() == BLACK ? IsWhite : IsBlack);
  bool strange=false;
  //if(position().col()==3 && position().row()==6) strange=true;
  for (const auto & elem : non_capture_moves)
  {
    auto state = board.GetState(position()+elem);
    if (!IsIllegal(state) && IsEmpty(state))
    {
      Move m { position(), position() + elem };
      moves.push_back(m);
      if(strange) std::cout << "Adding1"
                              << m.orig_pos
                              << " " << m.dest_pos  << std::endl;
    }
  }
  for (const auto&elem : capture_moves)
  {
    auto state = board.GetState(position() + elem);
    if (!IsIllegal(state) && IsOppColour(state))
    {
      Move m { position(), position() + elem };
      moves.push_back(m);
      if(strange) std::cout << "Adding2"
                              << m.orig_pos
                              << " " << m.dest_pos  << std::endl;

    }
  }
}


Pawn* Pawn::Create(const Position& pos) const
{
  return new Pawn(colour(), pos);
}

Pawn* Pawn::Clone() const
{
  return new Pawn(colour(), position());
}

Knight::Knight(Colour c, const Position& p)
  : Piece(c, p)
{

}

void Knight::GenMoves(const Board& board, std::vector<Move> & moves) const
{
  static Direction knight_moves[]
  {
    Direction(1, 2),
    Direction(1, -2),
    Direction(-1, -2),
    Direction(-1, 2),
    Direction(2, 1),
    Direction(2, -1),
    Direction(-2, 1),
    Direction(-2, -1)
  };
  auto IsOppColour = (colour() == BLACK ? IsWhite : IsBlack);
  for (const auto & elem : knight_moves)
  {
    auto state = board.GetState(position() + elem);
    if (!IsIllegal(state))
    {
      if (IsEmpty(state))
      {
        Move m { position(), position() + elem };
		//std::cout << m.dest_pos << " " << m.orig_pos << std::endl;
        moves.push_back(m);
      }
      else if (IsOppColour(state))
      {
        Move m { position(), position() + elem };
		//std::cout << m.dest_pos << " " << m.orig_pos << std::endl;
        moves.push_back(m);
      }

    }

  }
}

Knight* Knight::Create(const Position& pos) const
{
  return new Knight(colour(), pos);
}

Knight* Knight::Clone() const
{
  return new Knight(colour(), position());
}

Bishop::Bishop(Colour c, const Position& p)
  : Piece(c, p)
{

}

void Bishop::GenMoves(const Board& board, std::vector<Move>& moves) const
{
  Direction bishop_dirs[] { Direction{ 1, 1 }, Direction{ -1, -1 },
                            Direction{ -1, 1 }, Direction{ 1, -1 }
                          };
  auto IsOppColour = (colour() == BLACK ? IsWhite : IsBlack);
  for (const auto & elem : bishop_dirs)
  {
    auto orig_position = position();
    auto check_position = position() + elem;
    auto state = board.GetState(check_position);
    while (!IsIllegal(state) && IsEmpty(state))
    {
      Move m { orig_position, check_position};
      moves.push_back(m);
      check_position += elem;
      state = board.GetState(check_position);
    }
    if (!IsIllegal(state) && IsOppColour(state))
    {
      Move m { orig_position, check_position};
      moves.push_back(m);
    }
  }
}

Bishop* Bishop::Create(const Position& pos) const
{
  return new Bishop(colour(), pos);
}

Bishop* Bishop::Clone() const
{
  return new Bishop(colour(), position());
}

Rook::Rook(Colour c, const Position& p)
  : Piece(c, p)
{

}

void Rook::GenMoves(const Board& board, std::vector<Move>& moves) const
{
  Direction rook_dirs[]
  {
    Direction{ 1, 0 }, Direction{ -1, 0 },
    Direction{ 0, 1 }, Direction{ 0, -1 }
  };
  auto IsOppColour = (colour() == BLACK ? IsWhite : IsBlack);
  for (const auto & elem : rook_dirs)
  {
    auto orig_position = position();
    auto check_position = position() + elem;
    auto state = board.GetState(check_position);
    while (!IsIllegal(state) && IsEmpty(state))
    {
      Move m { orig_position, check_position };
      moves.push_back(m);
      check_position += elem;
      state = board.GetState(check_position);
    }
    if (!IsIllegal(state) && IsOppColour(state))
    {
      Move m { orig_position, check_position  };
      moves.push_back(m);
    }
  }
}

Rook* Rook::Create(const Position& pos) const
{
  return new Rook(colour(), pos);
}

Rook* Rook::Clone() const
{
  return new Rook(colour(), position());
}

King::King(Colour c, const Position& p)
  : Piece(c, p)
{

}

void King::GenMoves(const Board& board, std::vector<Move>& moves) const
{
  Direction king_dirs[]
  {
    Direction{ 1, 1 }, Direction{ -1, -1 },
    Direction{ -1, 1 }, Direction{ 1, -1 },
    Direction{ 1, 0 }, Direction{ -1, 0 },
    Direction{ 0, 1 }, Direction{ 0, -1 }
  };
  auto IsOppColour = (colour() == BLACK ? IsWhite : IsBlack);
  for (const auto & elem : king_dirs)
  {
    auto check_position = position() + elem;
    auto state = board.GetState(check_position);
    if (!IsIllegal(state) && IsEmpty(state))
    {
      Move m { position(), check_position };
      moves.push_back(m);
    }
    if (!IsIllegal(state) && IsOppColour(state))
    {
      Move m { position(), check_position };
      moves.push_back(m);
    }
  }
}

King* King::Create(const Position& pos) const
{
  return new King(colour(), pos);
}

King* King::Clone() const
{
  return new King(colour(), position());
}

Queen::Queen(Colour c, const Position& p)
  : Piece(c, p), Bishop(c, p), Rook(c, p)
{

}

void Queen::GenMoves(const Board& board, std::vector<Move>& moves) const
{
  Bishop::GenMoves(board, moves);
  Rook::GenMoves(board, moves);
}


Queen* Queen::Create(const Position& pos) const
{
  return new Queen(colour(), pos);
}

Queen* Queen::Clone() const
{
  return new Queen(colour(), position());
}
