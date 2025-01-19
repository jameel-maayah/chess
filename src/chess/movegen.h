#include <vector>

#include "chess.h"
#include "move.h"
#include "bitboard.h"

std::vector<Move> generate_moves(Chess *game);
int perft(Chess *game, int depth);
int detail_perft(Chess *game, int depth);