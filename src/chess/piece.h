#pragma once

#include <string>

#include "types.h"

class Piece {
    public:
        enum Type : uint8_t { // todo: store colorless pieces and let bit 4 represent color
            EMPTY = 0b0000,

            WHITE_PAWN = 0b0001,
            WHITE_KNIGHT = 0b0010,
            WHITE_BISHOP = 0b0011,
            WHITE_ROOK = 0b0100,
            WHITE_QUEEN = 0b0101,
            WHITE_KING = 0b0110,

            BLACK_PAWN = 0b1001,
            BLACK_KNIGHT = 0b1010,
            BLACK_BISHOP = 0b1011,
            BLACK_ROOK = 0b1100,
            BLACK_QUEEN = 0b1101,
            BLACK_KING = 0b1110
        };

        constexpr Piece() : internal(EMPTY) { }
        constexpr Piece(Type type) : internal(type) { }

        [[nodiscard]] constexpr inline Type type() const                            {   return internal;    }
        [[nodiscard]] constexpr inline Color color() const                          {   return static_cast<Color>(internal >> 3);   }
        [[nodiscard]] constexpr inline bool empty() const                           {   return (internal == 0x0);   }
        [[nodiscard]] constexpr inline Piece operator~() const                      {   return Piece(static_cast<Type>(internal ^ 0b1000));    }
        [[nodiscard]] constexpr inline bool operator==(const Piece& other) const    {   return (internal == other.internal);    }

        explicit operator std::string() const {
            switch (internal) {
                case WHITE_PAWN: return "P";
                case WHITE_KNIGHT: return "N";
                case WHITE_BISHOP: return "B";
                case WHITE_ROOK: return "R";
                case WHITE_QUEEN: return "Q";
                case WHITE_KING: return "K";
                case BLACK_PAWN: return "p";
                case BLACK_KNIGHT: return "n";
                case BLACK_BISHOP: return "b";
                case BLACK_ROOK: return "r";
                case BLACK_QUEEN: return "q";
                case BLACK_KING: return "k";
                case EMPTY: return ".";
            }
        }

        explicit operator std::wstring() const {
            switch (internal) {
                case WHITE_PAWN: return L"♟";
                case WHITE_KNIGHT: return L"♞";
                case WHITE_BISHOP: return L"♝";
                case WHITE_ROOK: return L"♜";
                case WHITE_QUEEN: return L"♛";
                case WHITE_KING: return L"♚";
                case BLACK_PAWN: return L"♟";
                case BLACK_KNIGHT: return L"♞";
                case BLACK_BISHOP: return L"♝";
                case BLACK_ROOK: return L"♜";
                case BLACK_QUEEN: return L"♛";
                case BLACK_KING: return L"♚";
                case EMPTY: return L".";
            }
        }

    private:
        Type internal;
};

