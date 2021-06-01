#include "mode.hpp"

#include <stdexcept>
#include <unordered_map>

namespace
{
    using namespace vind::mode ;
    const std::unordered_map<Mode, std::string> g_modeidxs {
        {Mode::Normal,          "guin"},
        {Mode::Insert,          "guii"},
        {Mode::Visual,          "guiv"},
        {Mode::Command,         "cmd"},
        {Mode::EdiNormal,       "edin"},
        {Mode::EdiInsert,       "edii"},
        {Mode::EdiVisual,       "ediv"},
        {Mode::EdiLineVisual,   "edivl"},
    } ;
}

namespace vind
{
    namespace mode
    {
        static Mode m{Mode::Normal} ;

        void change_mode(Mode mode) noexcept {
            m = mode ;
        }
        void change_mode(int mode) noexcept {
            m = static_cast<Mode>(mode) ;
        }
        void change_mode(unsigned char mode) noexcept {
            m = static_cast<Mode>(mode) ;
        }

        Mode get_global_mode() noexcept {
            return m ;
        }

        bool is_insert() noexcept {
            return m == Mode::Insert || m == Mode::EdiInsert ;
        }

        bool is_edi_visual() noexcept {
            return m == Mode::EdiVisual || m == Mode::EdiLineVisual ;
        }
        bool is_editor() noexcept {
            return m > Mode::_EditorModeThreshold ;
        }
        bool is_command() noexcept {
            return m == Mode::Command || m == Mode::EdiCommand ;
        }

        std::string get_mode_strcode(Mode mode) noexcept {
            try {
                return g_modeidxs.at(mode) ;
            }
            catch(const std::out_of_range&) {
                return std::string() ;
            }
        }

        Mode get_mode_from_strcode(const std::string& strcode) noexcept {
            static const auto obj = []() {
                std::unordered_map<std::string, Mode> um ;
                for(const auto& pair : g_modeidxs) {
                    um[pair.second] = pair.first ;
                }
                return um ;
            }() ;

            try {
                return obj.at(strcode) ;
            }
            catch(const std::out_of_range&) {
                return Mode::None ;
            }
        }
    }
}
