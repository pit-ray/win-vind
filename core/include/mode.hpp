#ifndef _MODE_HPP
#define _MODE_HPP

#include <string>

namespace vind
{
    namespace mode {
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


        constexpr std::size_t mode_num() noexcept {
            return static_cast<std::size_t>(Mode::NUM) ;
        }

        template <typename T>
        constexpr auto mode_name(const T mode) noexcept {
            switch(static_cast<Mode>(mode)) {
                case Mode::Normal:        return "GUI Normal" ;
                case Mode::Insert:        return "GUI Insert" ;
                case Mode::Visual:        return "GUI Visual" ;
                case Mode::Command:       return "Command" ;
                case Mode::EdiNormal:     return "Editor Normal" ;
                case Mode::EdiInsert:     return "Editor Insert" ;
                case Mode::EdiVisual:     return "Editor Visual" ;
                case Mode::EdiLineVisual: return "Editor Visual Line" ;
                default: return "Undefined" ;
            }
        }

        void change_mode(Mode mode) noexcept ;
        void change_mode(int mode) noexcept ;
        void change_mode(unsigned char mode) noexcept ;
        Mode get_global_mode() noexcept ;

        bool is_insert() noexcept ;

        bool is_edi_visual() noexcept ;
        bool is_editor() noexcept ;
        bool is_command() noexcept ;

        std::string get_mode_strcode(Mode mode) noexcept ;

        template <typename T>
        inline std::string get_mode_strcode(T mode) noexcept {
            return get_mode_strcode(static_cast<Mode>(mode)) ;
        }

        Mode get_mode_from_strcode(const std::string& strcode) noexcept ;
    }
}

#endif
