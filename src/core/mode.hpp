#ifndef _MODE_HPP
#define _MODE_HPP

#include <array>
#include <string>


namespace vind
{
    namespace core {
        enum ModeFlags : unsigned char {
            NONE        = 0x00,
            VISUAL_LINE = 0x01,
        } ;

        enum class Mode : unsigned char {
            INSERT,
            GUI_NORMAL,
            GUI_VISUAL,
            EDI_NORMAL,
            EDI_VISUAL,
            RESIDENT,
            COMMAND,
            NUM,
            UNDEFINED = 255,
        } ;

        inline constexpr auto mode_num() noexcept {
            return static_cast<std::size_t>(Mode::NUM) ;
        }

        void set_global_mode(
                Mode mode,
                ModeFlags flags=ModeFlags::NONE) noexcept ;
        template <typename T>
        inline void set_global_mode(
                T mode,
                ModeFlags flags=ModeFlags::NONE) noexcept {
            set_global_mode(static_cast<Mode>(mode), flags) ;
        }

        Mode get_global_mode() noexcept ;
        template <typename T>
        T get_global_mode() noexcept {
            return static_cast<T>(get_global_mode()) ;
        }

        ModeFlags get_global_mode_flags() noexcept ;

        std::string mode_to_prefix(Mode mode) noexcept ;
        template <typename T>
        inline std::string mode_to_prefix(T mode) noexcept {
            return mode_to_prefix(static_cast<Mode>(mode)) ;
        }

        Mode parse_mode_prefix(const std::string& prefix) ;

        const std::string& mode_to_name(Mode mode) noexcept ;
        template <typename T>
        inline const std::string& mode_to_name(T mode) noexcept {
            return mode_to_name(static_cast<Mode>(mode)) ;
        }



        template <typename T>
        using ModeArray = std::array<T, core::mode_num()> ;
    }
}

#endif
