#include <iostream>

#include <string_view>
#include <sstream>
#include <ranges>

#include <vector>

#include <wchar.h>
#include <locale.h>

#include "chess.h"
#include "bitboard.h"
#include "constants.h"
#include "types.h"
#include "piece.h"
#include "move.h"
#include "movegen.h"
#include "attacks.h"
#include "zobrist.h"

using std::vector;

Chess::Chess(std::string_view fen) {
    const std::string_view delimiter = " ";
    auto args = std::views::split(fen, delimiter);

    for (const auto& arg : args) {
        std::string token(arg.begin(), arg.end()); 
        //std::cout << token << std::endl;
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

    memset(bitboard, 0ULL, sizeof(bitboard));

    for (Square sq = a1; sq <= h8; ++sq) {
        bitboard::set_bit(bitboard[board[sq].type()], sq);
    }

    zobrist_history[ply] = Zobrist::init_hash(*this);
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

    zobrist_hash = Zobrist::update_hash(zobrist_hash, move, piece, capture);
    zobrist_history[ply] = zobrist_hash;
    
    int last_irreversible = halfmove_stack[halfmove_counter];
    int repetitions = 1;

    if (last_irreversible >= 8) { 
        for (int i = ply - 2; i >= ply - last_irreversible; i -= 2) {
            if (zobrist_history[i] == zobrist_hash) {
                ++repetitions;
            }
            if (repetitions >= 3) {
                status = Status::THREEFOLD;
                return;
            }
        }
    }
    
}

void Chess::undo_move() {
    Move move = move_history[--ply];
    Square source = move.source(),
           target = move.target();
    Piece piece = at(target),
          capture = captures[ply]; 

    board[source] = board[target];  
    board[target] = capture;

    bitboard::toggle_bit(bitboard[capture.type()], target);
    bitboard::toggle_bits(bitboard[piece.type()], source, target);

    if (!capture.empty() || 
        piece.type() == Piece::BLACK_PAWN || 
        piece.type() == Piece::WHITE_PAWN) {
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

    zobrist_hash = Zobrist::update_hash(zobrist_hash, move, piece, capture);
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
    const U64 occ = occupied_mask();
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
    std::cout << "Ply: " << ply << 
                 " Status: " << status << 
                 " Hash: " << std::to_string(zobrist_hash) << 
                 " Last move: " << std::string(move_history[ply-1]) << std::endl;

    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            Square sq = static_cast<Square>(rank * 8 + file);
            std::cout << std::string(board[sq]);
        }
        std::cout << std::endl;
    }
}

void Chess::fancy_print() {
    using std::cout;

    setlocale(LC_ALL, "");
    fwide(stdout, 1);

    //std::cout << ply << " " << status << " " << std::to_string(zobrist_hash) << " " << std::string(move_history[ply-1]) << std::endl;

    cout << "╭❖✤⚜━❃━❇━❃━❇━━❇━❃━❇━❃━⚜✤❖╮\n";

    for (int rank = 7; rank >= 0; --rank) {
        //printf("\033[0;37m");
        cout << "\033[0m";
        cout << rank + 1;

        for (int file = 0; file < 8; ++file) {
            Square sq = static_cast<Square>(rank * 8 + file);
            
            cout << "\033[0m";

            if ((rank + file) % 2 == 0) {
                cout << "\033[48;5;143m";//"\033[48;5;64m";//"\033[48;5;58m";//"\033[48;5;52m";
                //cout << "\033[48;5;81m";
            } else {
                cout << "\033[47m";//"\033[48;5;245m";
                //cout << "\033[48;5;227m";
            }

            if (!at(sq).empty()) {
                Piece piece = at(sq);

                if (piece.color() == WHITE) {
                    cout << "\033[38;5;136m";//"\033[38;5;220m";//"\033[38;5;222m";
                    //cout << "\033[38;5;1m";
                } else {
                    cout << "\033[38;5;94m";//"\033[116;58;52m";//"\033[38;5;52m";//"\033[38;5;94m";
                    //cout << "\033[38;5;12m";
                }

                wchar_t wc;

                switch (piece.type()) {
                    case Piece::WHITE_PAWN: wc = L'♟'; break;
                    case Piece::WHITE_KNIGHT: wc = L'♞'; break;
                    case Piece::WHITE_BISHOP: wc = L'♝'; break;
                    case Piece::WHITE_ROOK: wc = L'♜'; break;
                    case Piece::WHITE_QUEEN: wc = L'♛'; break;
                    case Piece::WHITE_KING: wc = L'♚'; break;
                    case Piece::BLACK_PAWN: wc = L'♟'; break;
                    case Piece::BLACK_KNIGHT: wc = L'♞'; break;
                    case Piece::BLACK_BISHOP: wc = L'♝'; break;
                    case Piece::BLACK_ROOK: wc = L'♜'; break;
                    case Piece::BLACK_QUEEN: wc = L'♛'; break;
                    case Piece::BLACK_KING: wc = L'♚'; break;
                    case Piece::EMPTY: wc = L'.'; break;
                }

                if (ply && move_history[ply - 1].target() == sq) {
                    cout << "\033[0m\033[42m ";
                    fputwc(wc, stdout);
                    cout << "\033[42m ";
                } else if ((sq == white_king || sq == black_king) && is_attacked(sq)) {
                    cout << "\033[0m\033[41m ";
                    if (get_status()) fputwc(L'☠', stdout); else fputwc(wc, stdout);
                    cout << "\033[41m ";
                } else {
                    cout << " ";
                    fputwc(wc, stdout);
                    cout << " ";
                }
            } else {
                if (ply && move_history[ply - 1].source() == sq) {
                    cout << "\033[0m";
                    cout << "\033[42m";
                }
                cout << "   ";
            }

        }

        cout << "\033[49m";
        //printf("\033[0;37m");
        cout << "\033[0m";
        cout << rank + 1 << std::endl;
    }

    cout << "\033[3m╰❇a❇✤b✤❃c❃✤d✤✤e✤❃f❃✤g✤❇h❇╯\033[0m\n";
    cout << "\e[0;32m\n";
}

void Chess::fancy_print(Color color) {
    using std::cout;

    if (color == WHITE) {
        return Chess::fancy_print();
    }

    setlocale(LC_ALL, "");
    fwide(stdout, 1);

    //std::cout << ply << " " << status << " " << std::to_string(zobrist_hash) << " " << std::string(move_history[ply-1]) << std::endl;

    cout << "╭❖✤⚜━❃━❇━❃━❇━━❇━❃━❇━❃━⚜✤❖╮\n";

    for (int rank = 0; rank < 8; ++rank) {
        //printf("\033[0;37m");
        cout << "\033[0m";
        cout << rank + 1;

        for (int file = 7; file >= 0; --file) {
            Square sq = static_cast<Square>(rank * 8 + file);
            
            cout << "\033[0m";

            if ((rank + file) % 2 == 0) {
                cout << "\033[48;5;143m";//"\033[48;5;64m";//"\033[48;5;58m";//"\033[48;5;52m";
            } else {
                cout << "\033[47m";//"\033[48;5;245m";
            }

            if (!at(sq).empty()) {
                Piece piece = at(sq);

                if (piece.color() == WHITE) {
                    cout << "\033[38;5;136m";//"\033[38;5;220m";//"\033[38;5;222m";
                } else {
                    cout << "\033[38;5;94m";//"\033[116;58;52m";//"\033[38;5;52m";//"\033[38;5;94m";
                }

                wchar_t wc;

                switch (piece.type()) {
                    case Piece::WHITE_PAWN: wc = L'♟'; break;
                    case Piece::WHITE_KNIGHT: wc = L'♞'; break;
                    case Piece::WHITE_BISHOP: wc = L'♝'; break;
                    case Piece::WHITE_ROOK: wc = L'♜'; break;
                    case Piece::WHITE_QUEEN: wc = L'♛'; break;
                    case Piece::WHITE_KING: wc = L'♚'; break;
                    case Piece::BLACK_PAWN: wc = L'♟'; break;
                    case Piece::BLACK_KNIGHT: wc = L'♞'; break;
                    case Piece::BLACK_BISHOP: wc = L'♝'; break;
                    case Piece::BLACK_ROOK: wc = L'♜'; break;
                    case Piece::BLACK_QUEEN: wc = L'♛'; break;
                    case Piece::BLACK_KING: wc = L'♚'; break;
                    case Piece::EMPTY: wc = L'.'; break;
                }

                if (ply && move_history[ply - 1].target() == sq) {
                    cout << "\033[0m\033[42m ";
                    fputwc(wc, stdout);
                    cout << "\033[42m ";
                } else if ((sq == white_king || sq == black_king) && is_attacked(sq)) {
                    cout << "\033[0m\033[41m ";
                    if (get_status()) fputwc(L'☠', stdout); else fputwc(wc, stdout);
                    cout << "\033[41m ";
                } else {
                    cout << " ";
                    fputwc(wc, stdout);
                    cout << " ";
                }
            } else {
                if (ply && move_history[ply - 1].source() == sq) {
                    cout << "\033[0m";
                    cout << "\033[42m";
                }
                cout << "   ";
            }

        }

        cout << "\033[49m";
        //printf("\033[0;37m");
        cout << "\033[0m";
        cout << rank + 1 << std::endl;
    }

    cout << "\033[3m╰❇h❇✤g✤❃f❃✤e✤✤d✤❃c❃✤b✤❇a❇╯\033[0m\n";
    cout << "\e[0;32m\n";
}