#pragma once

#include "types.h"

class Move {
    private:
        uint16_t repr;

    public:
        enum Flag : int {
            // may need more later

            QUIET = 0b0001,
            CAPTURE, // encode later
            CHECK,
            DOUBLE_CHECK,
            PROMOTION_KNIGHT,
            PROMOTION_BISHOP,
            PROMOTION_ROOK,
            PROMOTION_QUEEN
        };

        inline Move() : repr( 0x0000 ) { };
        inline Move(Square source, Square target) : repr( 0x0000 ) {
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