#include "change_mode.hpp"

#include <windows.h>

#include "bind/emu/textsel.hpp"
#include "core/charlogger.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "options.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/mouse.hpp"
#include "util/rect.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        //ToGUINormal
        ToGUINormal::ToGUINormal()
        : BindedFuncVoid("to_gui_normal")
        {}
        void ToGUINormal::sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout) {
            using core::Mode ;

            auto m = core::get_global_mode() ;
            if(m == Mode::GUI_NORMAL) {
                return ;
            }

            if(m == Mode::GUI_VISUAL) {
                util::click(KEYCODE_MOUSE_LEFT) ; //release holding mouse
            }
            else if(m == Mode::EDI_VISUAL) {
                unselect() ; //release shifting
            }

            //When this function is called, binded key is down.
            //Thus, its key is needed to be up before absorbing key.
            auto& igate = core::InputGate::get_instance() ;
            igate.close_all_ports_with_refresh() ;
            igate.absorb() ;

            core::set_global_mode(Mode::GUI_NORMAL) ;
            if(vclmodeout) {
                opt::VCmdLine::print(opt::GeneralMessage("-- GUI NORMAL --")) ;
            }
        }

        //ToResident
        ToResident::ToResident()
        : BindedFuncVoid("to_resident")
        {}
        void ToResident::sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout) {
            auto& igate = core::InputGate::get_instance() ;
            igate.close_all_ports() ;
            igate.unabsorb() ;
            core::set_global_mode(core::Mode::RESIDENT) ;
            if(vclmodeout) {
                opt::VCmdLine::print(opt::GeneralMessage("-- RESIDENT --")) ;
            }
        }

        //ToGUIVisual
        ToGUIVisual::ToGUIVisual()
        : BindedFuncVoid("to_gui_visual")
        {}
        void ToGUIVisual::sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout) {
            core::set_global_mode(core::Mode::GUI_VISUAL) ;
            if(vclmodeout) {
                opt::VCmdLine::print(opt::GeneralMessage("-- GUI VISUAL --")) ;
            }
            util::press_mousestate(KEYCODE_MOUSE_LEFT) ;
        }

        // All instances share TextAreaScanner to keep staticity of sprocess.
        TextAreaScanner ToEdiNormal::scanner_ ;

        //ToEdiNormal
        ToEdiNormal::ToEdiNormal()
        : BindedFuncVoid("to_edi_normal")
        {
            opt::AsyncUIACacheBuilder::register_properties(
                    scanner_.get_properties()) ;
        }
        void ToEdiNormal::sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout) {
            using core::Mode ;
            auto mode = core::get_global_mode() ;
            if(mode == Mode::EDI_NORMAL) {
                return ;
            }
            if(mode == Mode::GUI_NORMAL) {
                util::click(KEYCODE_MOUSE_LEFT) ;
            }
            else if(mode == Mode::EDI_VISUAL) {
                unselect() ;
            }

            auto& igate = core::InputGate::get_instance() ;
            igate.close_all_ports_with_refresh() ;
            igate.absorb() ;

            core::set_global_mode(Mode::EDI_NORMAL) ;
            if(vclmodeout) {
                opt::VCmdLine::print(opt::GeneralMessage("-- EDI NORMAL --")) ;
            }

            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("autofocus_textarea").get<bool>()) {
                auto hwnd = util::get_foreground_window() ;
                auto pos = util::get_cursor_pos() ;
                focus_nearest_textarea(hwnd, pos, scanner_) ;
            }
        }

        //ToInsert
        ToInsert::ToInsert()
        : BindedFuncVoid("to_insert")
        {}
        void ToInsert::sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout) {
            using core::Mode ;
            if(core::get_global_mode() == Mode::GUI_NORMAL) {
                util::click(KEYCODE_MOUSE_LEFT) ;
            }

            auto& igate = core::InputGate::get_instance() ;
            igate.close_all_ports() ;
            igate.unabsorb() ;

            core::set_global_mode(Mode::INSERT) ;

            if(vclmodeout) {
                opt::VCmdLine::print(opt::GeneralMessage("-- INSERT --")) ;
            }
        }

        //ToEdiVisual
        ToEdiVisual::ToEdiVisual()
        : BindedFuncVoid("to_edi_visual")
        {}
        void ToEdiVisual::sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout) {
            select_words() ;
            core::set_global_mode(core::Mode::EDI_VISUAL) ;
            if(vclmodeout) {
                opt::VCmdLine::print(opt::GeneralMessage("-- EDI VISUAL --")) ;
            }
        }

        //ToEdiVisualLine
        ToEdiVisualLine::ToEdiVisualLine()
        : BindedFuncVoid("to_edi_visual_line")
        {}
        void ToEdiVisualLine::sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout) {
            select_line_EOL2BOL() ;
            core::set_global_mode(
                    core::Mode::EDI_VISUAL, core::ModeFlags::VISUAL_LINE) ;
            if(vclmodeout) {
                opt::VCmdLine::print(opt::GeneralMessage("-- EDI VISUAL LINE--")) ;
            }
        }
    }
}
