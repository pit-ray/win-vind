#ifndef _MODE_HPP
#define _MODE_HPP

#include <string>

namespace vind
{
    namespace mode {
        enum Mode : unsigned char {
            INSERT       = 0b1100'0000,

            GUI_NORMAL   = 0b0101'0001,
            GUI_VISUAL   = 0b0100'0010,

            EDI_NORMAL   = 0b1001'0011,
            EDI_VISUAL   = 0b1000'0100,

            COMMAND      = 0b1100'0101,

            RESIDENT     = 0b1100'0110,

            MODE_NUM     = 7,

            EDI_VISUAL_L = 0b1010'0100,

            MASK_INDEX   = 0b0000'1111,
            MASK_GMODE   = 0b1100'0000,

            MASK_GUI     = 0b0100'0000,
            MASK_EDI     = 0b1000'0000,

            UNDEFINED    = 255,
        } ;

        inline constexpr auto mode_num() noexcept {
            return static_cast<std::size_t>(Mode::MODE_NUM) ;
        }

        void set_global_mode(Mode mode) noexcept ;
        template <typename T>
        inline void set_global_mode(T mode) noexcept {
            set_global_mode(static_cast<Mode>(mode)) ;
        }

        Mode get_global_mode() noexcept ;
        inline Mode get_global_index() noexcept {
            return static_cast<Mode>(get_global_mode() & Mode::MASK_INDEX) ;
        }

        std::string to_prefix(Mode mode) noexcept ;
        template <typename T>
        inline std::string to_prefix(T mode) noexcept {
            return to_prefix(static_cast<Mode>(mode)) ;
        }

        Mode prefix_to_mode(const std::string& prefix) noexcept ;

        const std::string& to_name(Mode mode) noexcept ;
        template <typename T>
        inline const std::string& to_name(T mode) noexcept {
            return to_name(static_cast<Mode>(mode)) ;
        }
    }
}

#endif
