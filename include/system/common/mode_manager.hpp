#ifndef _MODE_MANAGER_HPP
#define _MODE_MANAGER_HPP

namespace ModeManager
{
    enum class Mode : unsigned char {
        Normal,
        Insert,
        Visual,
        Command,

        EdiNormal,
        EdiInsert,
        EdiVisual,
        EdiLineVisual,
        EdiCommand,
    } ;

    void change_mode(const Mode mode) noexcept ;
    Mode get_mode() noexcept ;

    bool init() ;
    bool update() ;
}

#endif