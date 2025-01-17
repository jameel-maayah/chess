#include <vector>

#include <iostream>
#include <string>

#include "chess.h"
#include "bitboard.h"
#include "constants.h"
#include "types.h"
#include "piece.h"
#include "move.h"
#include "attacks.h"


using std::vector;


void add_to_movelist(Chess *game, vector<Move> &moves, Move move) {

    /*
    game.pseudo_move(move);
    if (game->side_to_move() == BLACK && is_attacked(game, game->white_king)) {
        game.pseudo_undo(game);
        return;
    }
    if (game->side_to_move() == WHITE && is_attacked(game, game->black_king)) {
        game.pseudo_undo(game);
        return;
    }
    game.pseudo_undo(game);
    */
    moves.push_back(move);
}


vector<Move> generate_moves(Chess *game) {
    Piece piece;
    Square square;

    vector<Move> moves = vector<Move>();
    moves.reserve(MAX_MOVES);

    U64 occ = game->occupied();

    for (square = a1; square < h8; ++square) {//todo: iterate through occupied squares only w/bitmask
        piece = game->at(square);

        if (piece.empty() | (piece.color() != game->side_to_move())){
            continue;
        }

        U64 attacks = 0ULL;

        switch (piece.type()) {
            case Piece::WHITE_PAWN:
                //pawn attacks
                attacks = 0ULL;
                break;

            case Piece::WHITE_KNIGHT:
                attacks = Attacks::knight_attacks(square);
                break;

            case Piece::WHITE_BISHOP:
                attacks = Attacks::bishop_attacks(occ, square);
                break;

            case Piece::WHITE_ROOK:
                attacks = Attacks::rook_attacks(occ, square);
                break;
                
            case Piece::WHITE_QUEEN:
                attacks = Attacks::queen_attacks(occ, square);
                break;

            case Piece::WHITE_KING:
                attacks = Attacks::king_attacks(square);
                break;
        }

        while (attacks) {
            int rshift = __builtin_ctzll(attacks);
            attacks &= attacks - 1;
            Square dest = static_cast<Square>(rshift);
            //Square dest = bitboard::popLSB(attacks); // fix this
            if (!bitboard::get_bit(occ, dest) || game->at(dest).color() != game->side_to_move()) {
                add_to_movelist(game, moves, Move(square, dest));
            }
        }
                
    }

    
    if (moves.size()) {
        return moves;
    } else {;
        return {};
    }
}