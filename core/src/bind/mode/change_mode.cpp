#include "bind/mode/change_mode.hpp"

#include <windows.h>

#include "bind/emu/simple_text_selecter.hpp"
#include "bind/mode/options.hpp"
#include "err_logger.hpp"
#include "g_params.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "key/char_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "mode.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/winwrap.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    //ToGUINormal
    ToGUINormal::ToGUINormal()
    : BindedFuncCreator("to_gui_normal")
    {}
    void ToGUINormal::sprocess(bool vclmodeout) {
        using namespace mode ;

        auto m = get_global_mode() ;
        if(m == Mode::GUI_NORMAL) {
            return ;
        }

        if(m == Mode::GUI_VISUAL) {
            mouse::click(KEYCODE_MOUSE_LEFT) ; //release holding mouse
        }
        else if(m == Mode::EDI_VISUAL) {
            textselect::unselect() ; //release shifting
        }

        //When this function is called, binded key is down.
        //Thus, its key is needed to be up before absorbing key.
        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::absorb() ;

        set_global_mode(Mode::GUI_NORMAL) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- GUI NORMAL --") ;
        }
    }
    void ToGUINormal::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToGUINormal::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //ToResident
    ToResident::ToResident()
    : BindedFuncCreator("to_resident")
    {}
    void ToResident::sprocess(bool vclmodeout) {
        keyabsorber::close_all_ports() ;
        keyabsorber::unabsorb() ;
        mode::set_global_mode(mode::Mode::RESIDENT) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- RESIDENT --") ;
        }
    }
    void ToResident::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToResident::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //ToGUIVisual
    ToGUIVisual::ToGUIVisual()
    : BindedFuncCreator("to_gui_visual")
    {}

    void ToGUIVisual::sprocess(bool vclmodeout) {
        using namespace mode ;
        set_global_mode(Mode::GUI_VISUAL) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- GUI VISUAL --") ;
        }
        mouse::press(KEYCODE_MOUSE_LEFT) ;
    }
    void ToGUIVisual::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToGUIVisual::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }

    //ToEdiNormal
    ToEdiNormal::ToEdiNormal()
    : BindedFuncCreator("to_edi_normal")
    {}
    void ToEdiNormal::sprocess(bool vclmodeout) {
        using namespace mode ;
        auto mode = get_global_mode() ;
        if(mode == Mode::EDI_NORMAL) {
            return ;
        }
        if(mode == Mode::GUI_NORMAL) {
            mouse::click(KEYCODE_MOUSE_LEFT) ;
        }
        else if(mode == Mode::EDI_VISUAL) {
            textselect::unselect() ;
        }

        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::absorb() ;

        set_global_mode(Mode::EDI_NORMAL) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- EDI NORMAL --") ;
        }

        if(gparams::get_b("autofocus_textarea")) {
            auto hwnd = GetForegroundWindow() ;
            if(!hwnd) {
                throw RUNTIME_EXCEPT("There is no foreground window.") ;
            }

            Point2D pos ;
            if(!GetCursorPos(&(pos.data()))) {
                throw RUNTIME_EXCEPT("Could not get the cursor position.") ;
            }

            // All instances share TextAreaScanner to keep staticity of sprocess.
            static TextAreaScanner scanner ;
            options::focus_nearest_textarea(hwnd, pos, scanner) ;
        }
    }
    void ToEdiNormal::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToEdiNormal::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //ToInsert
    ToInsert::ToInsert()
    : BindedFuncCreator("to_insert")
    {}

    void ToInsert::sprocess(bool vclmodeout) {
        using namespace mode ;
        if(get_global_mode() == Mode::GUI_NORMAL) {
            mouse::click(KEYCODE_MOUSE_LEFT) ;
        }

        keyabsorber::close_all_ports() ;
        keyabsorber::unabsorb() ;
        set_global_mode(Mode::INSERT) ;

        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- INSERT --") ;
        }
    }
    void ToInsert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //ToEdiVisual
    ToEdiVisual::ToEdiVisual()
    : BindedFuncCreator("to_edi_visual")
    {}
    void ToEdiVisual::sprocess(bool vclmodeout) {
        using namespace mode ;

        textselect::select_words() ;
        set_global_mode(Mode::EDI_VISUAL) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- EDI VISUAL --") ;
        }
    }
    void ToEdiVisual::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToEdiVisual::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //ToEdiVisualLine
    ToEdiVisualLine::ToEdiVisualLine()
    : BindedFuncCreator("to_edi_visual_line")
    {}
    void ToEdiVisualLine::sprocess(bool vclmodeout) {
        using namespace mode ;

        textselect::select_line_EOL2BOL() ;
        set_global_mode(Mode::EDI_VISUAL, ModeFlags::VISUAL_LINE) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- EDI VISUAL LINE--") ;
        }
    }
    void ToEdiVisualLine::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToEdiVisualLine::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }
}
