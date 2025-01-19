#include <iostream>

#include <string_view>
#include <sstream>
#include <ranges>

#include <vector>

#include "chess.h"
#include "bitboard.h"
#include "constants.h"
#include "types.h"
#include "piece.h"
#include "move.h"
#include "movegen.h"
#include "attacks.h"

using std::vector;

Chess::Chess(std::string_view fen) {
    const std::string_view delimiter = " ";
    auto args = std::views::split(fen, delimiter);

    for (const auto& arg : args) {
        std::string token(arg.begin(), arg.end()); 
        std::cout << token << std::endl;
    }
    // todo: fen parsing

    memset(board, 0, sizeof(board)); // EMPTY
    
    for (Square sq = a2; sq <= h2; sq += Direction::E) {
        //board[sq] = WHITE_PAWN;
        board[sq] = Piece(Piece::WHITE_PAWN);
    }
    for (Square sq = a7; sq <= h7; sq += Direction::E) {
        //board[sq] = BLACK_PAWN;
        board[sq] = Piece(Piece::BLACK_PAWN);
    }
    /*
    board[a1] = WHITE_ROOK;
    board[b1] = WHITE_KNIGHT;
    board[c1] = WHITE_BISHOP;
    board[d1] = WHITE_QUEEN;
    board[e1] = WHITE_KING;
    board[f1] = WHITE_BISHOP;
    board[g1] = WHITE_KNIGHT;
    board[h1] = WHITE_ROOK;

    board[a8] = BLACK_ROOK;
    board[b8] = BLACK_KNIGHT;
    board[c8] = BLACK_BISHOP;
    board[d8] = BLACK_QUEEN;
    board[e8] = BLACK_KING;
    board[f8] = BLACK_BISHOP;
    board[g8] = BLACK_KNIGHT;
    board[h8] = BLACK_ROOK;
    */

    // ugly please fix
    
    board[a1] = Piece(Piece::WHITE_ROOK);
    board[b1] = Piece(Piece::WHITE_KNIGHT);
    board[c1] = Piece(Piece::WHITE_BISHOP);
    board[d1] = Piece(Piece::WHITE_QUEEN);
    board[e1] = Piece(Piece::WHITE_KING);
    board[f1] = Piece(Piece::WHITE_BISHOP);
    board[g1] = Piece(Piece::WHITE_KNIGHT);
    board[h1] = Piece(Piece::WHITE_ROOK);

    board[a8] = Piece(Piece::BLACK_ROOK);
    board[b8] = Piece(Piece::BLACK_KNIGHT);
    board[c8] = Piece(Piece::BLACK_BISHOP);
    board[d8] = Piece(Piece::BLACK_QUEEN);
    board[e8] = Piece(Piece::BLACK_KING);
    board[f8] = Piece(Piece::BLACK_BISHOP);
    board[g8] = Piece(Piece::BLACK_KNIGHT);
    board[h8] = Piece(Piece::BLACK_ROOK);
    
    white_king = e1;
    black_king = e8;

    ply = 0;

    move_history[0] = Move();
    captures[0] = Piece(Piece::EMPTY);

    halfmove_stack[0] = 0;
    halfmove_counter = 0;
    status = IN_PROGRESS;
    zobrist_hash = 0;

    memset(bitboard, 0ULL, sizeof(bitboard));

    for (Square sq = a1; sq <= h8; ++sq) {
        bitboard::set_bit(bitboard[board[sq].type()], sq);
        //zobrist_hash ^= zobrist_table[sq][board[sq].type()];
    }

    zobrist_history[ply] = zobrist_hash;
}

Chess::~Chess() { /* might need later? */ }

void Chess::make_move(const Move move) {
    Square source = move.source(),
           target = move.target();
    Piece piece =   at(source),
          capture = at(target);

    board[source] = Piece(Piece::EMPTY);
    board[target] = piece;

    bitboard::clear_bit(bitboard[capture.type()], target);
    
    /* // todo: rewrite this old C code
    Piece promoted_piece = EMPTY;
    
    switch (GET_FLAG(move)) {
        case NO_PROMOTION:
            game->board[IX(target_x,target_y)] = piece;

            break;

        case PROMOTION_KNIGHT:
            promoted_piece = (IS_WHITE(piece)) ? WHITE_KNIGHT : BLACK_KNIGHT;
            game->board[IX(target_x,target_y)] = promoted_piece; 
            
            break;
        case PROMOTION_BISHOP:
            promoted_piece = (IS_WHITE(piece)) ? WHITE_BISHOP : BLACK_BISHOP;
            game->board[IX(target_x,target_y)] = promoted_piece; 
        
            break;
        case PROMOTION_ROOK:
            promoted_piece = (IS_WHITE(piece)) ? WHITE_ROOK : BLACK_ROOK; 
            game->board[IX(target_x,target_y)] = promoted_piece;

            break;
        case PROMOTION_QUEEN:
            promoted_piece = (IS_WHITE(piece)) ? WHITE_QUEEN : BLACK_QUEEN;
            game->board[IX(target_x,target_y)] = promoted_piece;

            break;
    }
    
    if (promoted_piece) {
        
        if (IS_WHITE(piece)) {
            game->value_white += piece_value(game->board[IX(target_x,target_y)]) - 1;
        } else {
            game->value_black += piece_value(game->board[IX(target_x,target_y)]) - 1;
        }
        
        CLEAR_BIT(game->bitboard[BITBOARD_IX(piece)], IX(piece_x, piece_y));
        SET_BIT(game->bitboard[BITBOARD_IX(promoted_piece)], IX(target_x, target_y));
    } else {
        TOGGLE_BITS(game->bitboard[BITBOARD_IX(piece)], IX(piece_x, piece_y), IX(target_x, target_y));
    }
    */

    bitboard::toggle_bits(bitboard[piece.type()], source, target);

    move_history[ply] = move;
    captures[ply++] = capture;

    if (!capture.empty() || piece.type() == Piece::BLACK_PAWN || piece.type() == Piece::WHITE_PAWN) {
        halfmove_counter++;
        halfmove_stack[halfmove_counter] = 0;
    } else {
        halfmove_stack[halfmove_counter]++;
    }

    if (halfmove_stack[halfmove_counter] >= 100) {
        status = Status::FIFTY_MOVE;
        return;
    }
    
    if (piece.type() == Piece::WHITE_KING) {
        white_king = target;
    } else if (piece.type() == Piece::BLACK_KING) {
        black_king = target;
    }

    /* // castling, more old C code
    switch (piece) {
        case (WHITE_KING):
            if (move == 0b100000110000) {
                TOGGLE_BITS(game->bitboard[BITBOARD_IX(WHITE_ROOK)], IX(7,0), IX(5,0));
                game->board[IX(7,0)] = EMPTY;
                game->board[IX(5,0)] = WHITE_ROOK;
            } else if (move == 0b100000010000) {
                TOGGLE_BITS(game->bitboard[BITBOARD_IX(WHITE_ROOK)], IX(0,0), IX(3,0));
                game->board[IX(0,0)] = EMPTY;
                game->board[IX(3,0)] = WHITE_ROOK;
            }
            game->white_king_x = target_x;
            game->white_king_y = target_y;
            game->white_castling[0] = 0;
            game->white_castling[1] = 0;

            break;

        case (BLACK_KING):
            if (move == 0b100111110111) {
                TOGGLE_BITS(game->bitboard[BITBOARD_IX(BLACK_ROOK)], IX(7,7), IX(5,7));
                game->board[IX(7,7)] = EMPTY;
                game->board[IX(5,7)] = BLACK_ROOK;
            } else if (move == 0b100111010111) {
                TOGGLE_BITS(game->bitboard[BITBOARD_IX(BLACK_ROOK)], IX(0,7), IX(3,7));
                game->board[IX(0,7)] = EMPTY;
                game->board[IX(3,7)] = BLACK_ROOK;
            } 
            game->black_king_x = target_x;
            game->black_king_y = target_y;
            game->black_castling[0] = 0;
            game->black_castling[1] = 0;

            break;

        case (WHITE_ROOK):
            if (game->white_castling[0] && piece_x == 7 && piece_y == 0) {
                game->white_castling[0] = 0;
            } else if (game->white_castling[1] && piece_x == 0 && piece_y == 0) {
                game->white_castling[1] = 0;
            }
            break;

        case (BLACK_ROOK):
            if (game->black_castling[0] && piece_x == 7 && piece_y == 7) {
                game->black_castling[0] = 0;
            } else if (game->black_castling[1] && piece_x == 0 && piece_y == 7) {
                game->black_castling[1] = 0;
            }
            break;

    }
    */

    stm = ~stm;

    // todo: impl proper checkmate detection
    if (side_to_move() == WHITE) {
        vector<Move> moves = generate_moves(this); // todo: put in namespace/class?

        if (moves.size() == 0) {
            if (is_attacked(white_king)) {
                status = Status::WIN_BLACK;
            } else {
                status = Status::STALEMATE;
            }
        }
    } else if (side_to_move() == BLACK) {
        vector<Move> moves = generate_moves(this);

        if (moves.size() == 0) {
            if (is_attacked(black_king)) {
                status = Status::WIN_WHITE;
            } else {
                status = Status::STALEMATE;
            }
        }
    }

    /* 
    zobrist_hash ^= zobrist_table[source][piece];
    zobrist_hash ^= zobrist_table[target][piece];
    if (capture) {
        zobrist_hash ^= zobrist_table[target][capture];
    }

    zobrist_hash ^= zobrist_stm;
    zobrist_history[ply] = zobrist_hash;
    
    int last_irreversible = game->fifty_move[game->fifty_move_counter];
    int repetitions = 1;

    if (last_irreversible >= 8) { 
        for (int i = ply - 2; i >= ply - last_irreversible; i -= 2) {
            if (zobrist_history[i] == game->zobrist_hash) {
                ++repetitions;
            }
            if (repetitions >= 3) {
                game->status = Status::THREEFOLD;
                return;
            }
        }
    }
    */
}

void Chess::undo_move() {
    Move move = move_history[--ply];
    Square source = move.source(),
           target = move.target();
    Piece piece = at(target),
          capture = captures[ply]; 

    bitboard::toggle_bit(bitboard[capture.type()], target);
    board[source] = board[target];  
    bitboard::toggle_bits(bitboard[piece.type()], source, target);
    board[target] = capture;

    if (!capture.empty() || piece.type() == Piece::BLACK_PAWN || piece.type() == Piece::WHITE_PAWN) {
        halfmove_counter--;
    } else {
        halfmove_stack[halfmove_counter]--;
    }
    status = Status::IN_PROGRESS;

    if (piece.type() == Piece::WHITE_KING) {
        white_king = source;
    } else if (piece.type() == Piece::BLACK_KING) {
        black_king = source;
    }

    /*
    zobrist_hash ^= zobrist_table[IX(piece_x, piece_y)][piece + 6];
    zobrist_hash ^= zobrist_table[IX(target_x, target_y)][piece + 6];
    if (!capture.empty()) {
        zobrist_hash ^= zobrist_table[target][capture.type()];
    }

    zobrist_hash ^= zobrist_stm;
    */

    stm = ~stm;
}

void Chess::pseudo_move(const Move move) {
    Square source = move.source(),
           target = move.target();
    Piece piece =   at(source),
          capture = at(target);

    board[source] = Piece(Piece::EMPTY);
    board[target] = piece;

    bitboard::clear_bit(bitboard[capture.type()], target);
    bitboard::toggle_bits(bitboard[piece.type()], source, target);

    
    move_history[ply] = move;
    captures[ply++] = capture;

    if (piece.type() == Piece::WHITE_KING) {
        white_king = target;
    } else if (piece.type() == Piece::BLACK_KING) {
        black_king = target;
    }

    stm = ~stm;
}

void Chess::pseudo_undo() {
    Move move = move_history[--ply];

    Square source = move.source(),
           target = move.target();
    Piece piece = at(target),
          capture = captures[ply];

    bitboard::set_bit(bitboard[capture.type()], target);
    bitboard::toggle_bits(bitboard[piece.type()], source, target);

    board[source] = piece;
    board[target] = capture;

    if (piece.type() == Piece::WHITE_KING) {
        white_king = source;
    } else if (piece.type() == Piece::BLACK_KING) {
        black_king = source;
    }

    stm = ~stm;
}

// later could make this return a bitboard of attackers for check, double check etc
bool Chess::is_attacked(const Square sq) {
    const U64 occ = occupied();
    const Color victim_color = at(sq).color();

    if (victim_color == WHITE) {
        if (get_file(sq) != A_FILE && 
            get_rank(sq) != RANK_8 &&
            bitboard::get_bit(bitboard[Piece::BLACK_PAWN], sq + Direction::NW)) return true;

        if (get_file(sq) != H_FILE && 
            get_rank(sq) != RANK_8 &&
            bitboard::get_bit(bitboard[Piece::BLACK_PAWN], sq + Direction::NE)) return true;


        // short circuit evaluation if there are none of a piece

        // steppers
        if (bitboard[Piece::BLACK_KNIGHT] && 
            (bitboard[Piece::BLACK_KNIGHT] & Attacks::knight_attacks(sq))) return true;
        if (bitboard[Piece::BLACK_KING] && 
            (bitboard[Piece::BLACK_KING] & Attacks::king_attacks(sq))) return true;
        // todo: make pawns work

        // sliders
        U64 bishop_attacks = 0ULL;
        U64 rook_attacks = 0ULL;

        if (bitboard[Piece::BLACK_BISHOP]  | bitboard[Piece::BLACK_QUEEN]) {
            bishop_attacks = Attacks::bishop_attacks(occ, sq);
            if (bitboard[Piece::BLACK_BISHOP] & bishop_attacks) return true;
        }
        if (bitboard[Piece::BLACK_ROOK]  | bitboard[Piece::BLACK_QUEEN]) {
            rook_attacks = Attacks::rook_attacks(occ, sq);
            if (bitboard[Piece::BLACK_ROOK] & rook_attacks) return true;
        }

        U64 queen_attacks = bishop_attacks | rook_attacks;

        if (bitboard[Piece::BLACK_QUEEN] &&
            (bitboard[Piece::BLACK_QUEEN] & queen_attacks)) return true;

        return false;

    } else if (victim_color == BLACK) {
        if (get_file(sq) != A_FILE && 
            get_rank(sq) != RANK_1 &&
            bitboard::get_bit(bitboard[Piece::WHITE_PAWN], sq + Direction::SW)) return true;

        if (get_file(sq) != H_FILE && 
            get_rank(sq) != RANK_1 &&
            bitboard::get_bit(bitboard[Piece::WHITE_PAWN], sq + Direction::SE)) return true;

        // steppers
        if (bitboard[Piece::WHITE_KNIGHT] && 
            (bitboard[Piece::WHITE_KNIGHT] & Attacks::knight_attacks(sq))) return true;
        if (bitboard[Piece::WHITE_KING] && 
            (bitboard[Piece::WHITE_KING] & Attacks::king_attacks(sq))) return true;

        // sliders
        U64 bishop_attacks = 0ULL;
        U64 rook_attacks = 0ULL;

        if (bitboard[Piece::WHITE_BISHOP] | bitboard[Piece::WHITE_QUEEN]) {
            bishop_attacks = Attacks::bishop_attacks(occ, sq);
            if (bitboard[Piece::WHITE_BISHOP] & bishop_attacks) return true;
        }
        if (bitboard[Piece::WHITE_ROOK] | bitboard[Piece::WHITE_QUEEN]) {
            rook_attacks = Attacks::rook_attacks(occ, sq);
            if (bitboard[Piece::WHITE_ROOK] & rook_attacks) return true;
        }

        U64 queen_attacks = bishop_attacks | rook_attacks;

        if (bitboard[Piece::WHITE_QUEEN] &&
            (bitboard[Piece::WHITE_QUEEN] & queen_attacks)) return true;

        return false;

    }
    return false;
}


void Chess::print() {
    //printf("ply %d, status %d, hash %d, lastmove %s\n", ply, status, zobrist_hash, 
    std::cout << ply << " " << status << " " << std::to_string(zobrist_hash) << " " << std::string(move_history[ply-1]) << std::endl;

    // todo: stop printing board upside down
    for (Square sq = a1; sq < NULL_SQ; ++sq) {
        std::cout << std::string(board[sq]);
        if (int(sq+1) % 8 == 0) std::cout << std::endl;
    }
    // todo: ascii piece representations
}

void Chess::fancy_print() {

}