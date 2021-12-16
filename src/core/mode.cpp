#include "mode.hpp"

#include <array>
#include <stdexcept>
#include <unordered_map>


namespace
{
    using namespace vind::core ;

    auto& global_mode() noexcept {
        static auto instance = Mode::INSERT ;
        return instance ;
    }

    auto& global_mode_flags() noexcept {
        static auto instance = ModeFlags::NONE ;
        return instance ;
    }
}


namespace vind
{
    namespace core
    {
        void set_global_mode(Mode mode, ModeFlags flags) noexcept {
            global_mode() = mode ;
            global_mode_flags() = flags ;
        }

        Mode get_global_mode() noexcept {
            return global_mode() ;
        }
        ModeFlags get_global_mode_flags() noexcept {
            return global_mode_flags() ;
        }

        std::string mode_to_prefix(Mode mode) noexcept {
            static auto to_prefix = [] {
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
            return to_prefix[static_cast<int>(mode)] ;
        }

        Mode parse_mode_prefix(const std::string& prefix) noexcept {
            static std::unordered_map<std::string, Mode> to_mode {
                {"i",  Mode::INSERT},
                {"gn", Mode::GUI_NORMAL},
                {"gv", Mode::GUI_VISUAL},
                {"en", Mode::EDI_NORMAL},
                {"ev", Mode::EDI_VISUAL},
                {"c",  Mode::COMMAND},
                {"r",  Mode::RESIDENT}
            } ;

            try {
                return to_mode.at(prefix) ;
            }
            catch(const std::out_of_range&) {
                return Mode::UNDEFINED ;
            }
        }

        const std::string& mode_to_name(Mode mode) noexcept {
            static const std::unordered_map<Mode, std::string> obj {
                {Mode::INSERT,     "Insert"},
                {Mode::GUI_NORMAL, "GUI Normal"},
                {Mode::GUI_VISUAL, "GUI Visual"},
                {Mode::EDI_NORMAL, "Editor Normal"},
                {Mode::EDI_VISUAL, "Editor Visual"},
                {Mode::INSERT,     "Insert"},
                {Mode::COMMAND,    "Command"},
                {Mode::RESIDENT,   "Resident"}
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
