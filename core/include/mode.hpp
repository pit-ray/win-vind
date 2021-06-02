#ifndef _MODE_HPP
#define _MODE_HPP

#include <array>
#include <stdexcept>
#include <string>


namespace vind
{
    namespace mode {
        enum Mode : unsigned char {
            INSERT       = 0b1100'0000,

            GUI_NORMAL   = 0b0101'0001,
            GUI_VISUAL   = 0b0110'0010,

            EDI_NORMAL   = 0b1001'0011,
            EDI_VISUAL   = 0b1010'0100,
            EDI_VISUAL_L = 0b1010'0101,

            COMMAND      = 0b1100'0110,

            MODE_NUM     = 0b0000'0111,

            MASK_INDEX   = 0b0000'1111,
            MASK_GUI     = 0b0001'0000,
            MASK_EDI     = 0b0010'0000,
            MASK_NORMAL  = 0b0010'0001,
            MASK_VISUAL  = 0b0010'0010,

            UNDEFINED    = 255,
        } ;

        template <typename T>
        constexpr auto mode_name(const T mode) noexcept {
            constexpr auto obj = [] {
                std::array<std::string, mode::MODE_NUM> ar{} ;
                ar.fill("Undefined") ;

                ar[Mode::INSERT]       = "Insert" ;

                ar[Mode::GUI_NORMAL]   = "GUI Normal" ;
                ar[Mode::GUI_VISUAL]   = "GUI Visual" ;

                ar[Mode::EDI_NORMAL]   = "Editor Normal" ;
                ar[Mode::EDI_VISUAL]   = "Editor Visual" ;
                ar[Mode::EDI_VISUAL_L] = "Editor Visual Line" ;

                ar[Mode::COMMAND]      = "Command" ;

                return ar ;
            }() ;

            return obj[mode] ;
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
