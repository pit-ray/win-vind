#ifndef _MODE_MANAGER_HPP
#define _MODE_MANAGER_HPP

namespace ModeManager
{
    enum class Mode : unsigned char {
        Normal,
        Insert,
        Visual,
        Command,

        _EditorModeThreshold,
        EdiNormal,
        EdiInsert,
        EdiVisual,
        EdiLineVisual,
        EdiCommand,

        NUM,
        None = 255
    } ;

    template <typename T>
    constexpr auto mode_name(const T mode) noexcept {
        switch(static_cast<Mode>(mode)) {
            case Mode::Normal:          return "GUI Normal" ;
            case Mode::Insert:          return "GUI Insert" ;
            case Mode::Visual:          return "GUI Visual" ;
            case Mode::Command:         return "Command" ;
            case Mode::EdiNormal:       return "Editor Normal" ;
            case Mode::EdiInsert:       return "Editor Insert" ;
            case Mode::EdiVisual:       return "Editor Visual" ;
            case Mode::EdiLineVisual:   return "Editor Visual Line" ;
            default:                    return "Undefined" ;
        }
    }

    void change_mode(const Mode mode) noexcept ;
    void change_mode(const int mode) noexcept ;
    void change_mode(const unsigned char mode) noexcept ;
    Mode get_mode() noexcept ;

    bool is_edi_visual() noexcept ;
    bool is_editor() noexcept ;

    bool is_command() noexcept ;
}

#endif
