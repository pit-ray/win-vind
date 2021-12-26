#ifndef _DEFS_HPP
#define _DEFS_HPP

namespace vind
{
    enum class SystemCall : unsigned char {
        NOTHING     = 0b0000'0000,
        TERMINATE   = 0b0000'0001,
        RECONSTRUCT = 0b0000'0010
    } ;
}

#endif
