#include "change_mode.hpp"

#include <windows.h>

#include "bind/emu/simple_text_selecter.hpp"
#include "core/char_logger.hpp"
#include "core/err_logger.hpp"
#include "core/g_params.hpp"
#include "core/key_absorber.hpp"
#include "core/keycode_def.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "opt/async_uia_cache_builder.hpp"
#include "opt/vcmdline.hpp"
#include "options.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/mouse.hpp"
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
            VCmdLine::print(GeneralMessage("-- GUI NORMAL --")) ;
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
            VCmdLine::print(GeneralMessage("-- RESIDENT --")) ;
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
            VCmdLine::print(GeneralMessage("-- GUI VISUAL --")) ;
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


    // All instances share TextAreaScanner to keep staticity of sprocess.
    TextAreaScanner ToEdiNormal::scanner_ ;

    //ToEdiNormal
    ToEdiNormal::ToEdiNormal()
    : BindedFuncCreator("to_edi_normal")
    {
        AsyncUIACacheBuilder::register_properties(scanner_.get_properties()) ;
    }
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
            VCmdLine::print(GeneralMessage("-- EDI NORMAL --")) ;
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

            options::focus_nearest_textarea(hwnd, pos, scanner_) ;
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
            VCmdLine::print(GeneralMessage("-- INSERT --")) ;
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
            VCmdLine::print(GeneralMessage("-- EDI VISUAL --")) ;
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
            VCmdLine::print(GeneralMessage("-- EDI VISUAL LINE--")) ;
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
