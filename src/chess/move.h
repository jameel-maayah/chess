#pragma once

#include "types.h"

enum Flag : int {
    // may need more later

    QUIET = 0b0001,
    CAPTURE,
    CHECK,
    DOUBLE_CHECK,
    PROMOTION_KNIGHT,
    PROMOTION_BISHOP,
    PROMOTION_ROOK,
    PROMOTION_QUEEN
};

class Move {
    private:
        uint16_t repr;

    public:
        inline Move() : repr( NULL ) { };
        inline Move(Square source, Square target) : repr( NULL ) {
            repr = (source << 6) | (target);
        };

        inline Move(Square source, Square target, Flag flag) : repr( NULL ) {
            repr = (flag << 12) | (source << 6) | (target);
        };

        inline Square source()              {   return static_cast<Square>(repr & 0x0FC0);     }
        inline Square target()              {   return static_cast<Square>(repr & 0x003F);     }
        inline Flag flag()                  {   return static_cast<Flag>(repr & 0xF000);       }
        inline void set_flag(Flag flag)     {   repr |= (flag << 12);                          }

        // todo: overload < > for move ordering
};