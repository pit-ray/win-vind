#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

namespace System
{
    enum class Mode : unsigned char {
        Normal,
        Insert,
        Visual,
        EdiNormal,
        EdiInsert,
        EdiVisual,
        Command,
    } ;

    void change_mode(const Mode mode) noexcept ;
    Mode get_mode() noexcept ;
}

#endif