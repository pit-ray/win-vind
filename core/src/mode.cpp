#include "mode.hpp"

#include <array>
#include <stdexcept>
#include <unordered_map>


namespace
{
    using namespace vind::mode ;
    static std::unordered_map<Mode, std::string> g_mode_prefix {
        {Mode::INSERT,     "i"},
        {Mode::GUI_NORMAL, "gn"},
        {Mode::GUI_VISUAL, "gv"},
        {Mode::EDI_NORMAL, "en"},
        {Mode::EDI_VISUAL, "ev"},
        {Mode::COMMAND,    "c"}
    } ;

    static Mode g_mode{Mode::INSERT} ;
}


namespace vind
{
    namespace mode
    {
        void set_global_mode(Mode mode) noexcept {
            g_mode = mode ;
        }

        Mode get_global_mode() noexcept {
            return g_mode ;
        }

        std::string to_prefix(Mode mode) noexcept {
            try {
                return g_mode_prefix.at(mode) ;
            }
            catch(const std::out_of_range&) {
                return "" ;
            }
        }

        Mode prefix_to_mode(const std::string& prefix) noexcept {
            static auto obj = [] {
                std::unordered_map<std::string, Mode> um ;
                for(auto& [key, val] : g_mode_prefix) {
                    um[val] = key ;
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
                {Mode::EDI_VISUAL_L, "Editor Visual Line"},
                {Mode::INSERT,       "Insert"},
                {Mode::COMMAND,      "Command"}
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
