#include "mode.hpp"

#include <array>
#include <stdexcept>
#include <unordered_map>


namespace
{
    using namespace vind::mode ;
    const auto g_mode_prefix = [] {
        std::array<std::string, mode_num()> tmp ;
        tmp[static_cast<int>(Mode::INSERT)]     = "i" ;
        tmp[static_cast<int>(Mode::GUI_NORMAL)] = "gn" ;
        tmp[static_cast<int>(Mode::GUI_VISUAL)] = "gv" ;
        tmp[static_cast<int>(Mode::EDI_NORMAL)] = "en" ;
        tmp[static_cast<int>(Mode::EDI_VISUAL)] = "ev" ;
        tmp[static_cast<int>(Mode::COMMAND)]    = "c" ;
        tmp[static_cast<int>(Mode::RESIDENT)]   = "r" ;
        return tmp ;
    } () ;

    auto g_mode  = Mode::INSERT ;
    auto g_flags = ModeFlags::NONE ;
}


namespace vind
{
    namespace mode
    {
        void set_global_mode(Mode mode, ModeFlags flags) noexcept {
            g_mode = mode ;
            g_flags = flags ;
        }

        Mode get_global_mode() noexcept {
            return g_mode ;
        }
        ModeFlags get_global_flags() noexcept {
            return g_flags ;
        }

        std::string to_prefix(Mode mode) noexcept {
            return g_mode_prefix[static_cast<int>(mode)] ;
        }

        Mode parse_prefix(const std::string& prefix) noexcept {
            static auto obj = [] {
                std::unordered_map<std::string, Mode> um ;
                for(std::size_t i = 0 ; i < mode_num() ; i ++) {
                    um[g_mode_prefix[i]] = static_cast<Mode>(i) ;
                }
                return um ;
            }() ;

            try {
                return obj.at(prefix) ;
            }
            catch(const std::out_of_range&) {
                return Mode::UNDEFINED ;
            }
        }

        const std::string& to_name(Mode mode) noexcept {
            static const std::unordered_map<Mode, std::string> obj {
                {Mode::INSERT,       "Insert"},
                {Mode::GUI_NORMAL,   "GUI Normal"},
                {Mode::GUI_VISUAL,   "GUI Visual"},
                {Mode::EDI_NORMAL,   "Editor Normal"},
                {Mode::EDI_VISUAL,   "Editor Visual"},
                {Mode::INSERT,       "Insert"},
                {Mode::COMMAND,      "Command"},
                {Mode::RESIDENT,     "Resident"}
            } ;

            try {
                return obj.at(mode) ;
            }
            catch(const std::out_of_range&) {
                static const std::string undef_str("Undefined") ;
                return undef_str ;
            }
        }
    }
}
