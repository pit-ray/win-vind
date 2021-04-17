#include "bind/emu/edi_edit.hpp"

#include <stdio.h>
#include <windows.h>

#include <memory>   //for std::unique_ptr
#include <iostream> //for debug

#include "bind/base/mode.hpp"
#include "bind/base/ntype_logger.hpp"
#include "bind/bind.hpp"
#include "bind/emu/edi_change_mode.hpp"
#include "bind/func_finder.hpp"
#include "coreio/err_logger.hpp"
#include "coreio/i_params.hpp"
#include "entry.hpp"
#include "io/keybrd.hpp"
#include "key/key_absorber.hpp"
#include "key/keycodecvt.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "text/simple_text_selecter.hpp"
#include "text/smart_clipboard.hpp"
#include "text/text_analyzer.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"

namespace
{
    using namespace vind ;

    enum class RegisteredType : unsigned char {
        None,
        Chars,
        Lines,
    } ;
    auto g_rgtype = RegisteredType::None ;

    //Some editors have a visible EOL mark in a line.
    //This function select text from current position to EOL except for the visible EOL mark.
    //If the line has only null characters, it does not select.
    //  <EOL mark exists> [select] NONE    [clipboard] null characters with EOL.    (neighborhoods of LSB are 0x00)
    //  <plain text>      [select] NONE    [clipboard] null characters without EOL. (neighborhoods of LSB are 0x?0)
    bool select_line_until_EOL(const textanalyze::SelRes* const exres) {
        using namespace keybrd ;
        if(exres != nullptr) {
            pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
            if(exres->having_EOL) {
                pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                if(exres->str.empty()) {
                    return false ; //not selected (true text is only null text)
                }
            }
            return true ; //selected
        }

        auto res = textanalyze::get_selected_text([] {
                pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        }) ;
        if(res.having_EOL) {
            pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
            if(res.str.empty()) {
                return false ;
            }
        }
        return true ;
    }

    inline void clear_clipboard_with_null() {
        const auto hwnd = GetForegroundWindow() ;
        if(!hwnd) {
            throw RUNTIME_EXCEPT("not exist active window") ;
        }
        SmartClipboard scb(hwnd) ;
        scb.open() ;
        scb.set("") ;
        scb.close() ;
    }

    bool select_by_motion(std::size_t caller_self_id, FuncFinder& ff) {
        NTypeLogger lgr ;
        keyabsorber::InstantKeyAbsorber ika ;

        while(vind::update_background()) {
            auto result = lgr.logging_state() ;
            if(NTYPE_EMPTY(result)) {
                continue ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                VirtualCmdLine::cout(std::to_string(lgr.get_head_num())) ;
                continue ;
            }

            auto parser = ff.find_parser(lgr.latest(), caller_self_id, mode::Mode::EdiLineVisual) ;
            if(parser && parser->get_func()->is_for_moving_caret()) {
                if(parser->is_accepted()) {
                    mode::change_mode(mode::Mode::EdiLineVisual) ;
                    parser->get_func()->process(lgr) ;
                    return true ;
                }
                else if(parser->is_rejected_with_ready()) {
                    ff.undo_parsers(1) ;
                    lgr.remove_from_back(1) ;
                }
            }
            else {
                break ;
            }
        }

        return false ;
    }

    //The parent logger is stronger than the child logger.
    //For example, the child BindedFunc calling this function is binded with 'c{motion}'
    //and 'cc' are bindings EdiDeleteLinesAndStartInsert.
    //In this case, if a child process has a message loop, we must consider the parent logger by full scanning.
    //
    // True : called with motion
    // False: called with parent_caller
    //
    bool select_by_motion(
            std::size_t caller_self_id,
            FuncFinder& ff,
            FuncFinder&parent_ff,
            NTypeLogger& parent_lgr) {

        const auto vmode = mode::Mode::EdiLineVisual ;

        ff.reset_parsers(vmode) ;
        parent_ff.reset_parsers() ;
        parent_ff.transition_parser_states_in_batch(parent_lgr) ;

        NTypeLogger lgr ;

        keyabsorber::InstantKeyAbsorber ika ;

        while(vind::update_background()) {
            auto result = lgr.logging_state() ;
            parent_lgr.logging_state() ;
            if(NTYPE_EMPTY(result)) {
                continue ;
            }
            if(NTYPE_HEAD_NUM(result)) {
                VirtualCmdLine::cout(std::to_string(lgr.get_head_num())) ;
                parent_lgr.remove_from_back(1) ;
                continue ;
            }

            auto parser_parent = parent_ff.find_parser(lgr.latest(), caller_self_id) ;
            if(parser_parent) {
                if(parser_parent->get_func()->id() == caller_self_id) {
                    parser_parent = nullptr ;
                }
                else if(parser_parent->is_accepted()) {
                    VirtualCmdLine::reset() ;
                    parser_parent->get_func()->process(parent_lgr) ;
                    return false ;
                }
            }

            auto parser = ff.find_parser(lgr.latest(), caller_self_id, vmode) ;
            if(parser && parser->get_func()->is_for_moving_caret()) {
                if(parser->is_accepted()) {
                    mode::change_mode(vmode) ;
                    for(std::size_t i = 0 ; i < parent_lgr.get_head_num() ; i ++) {
                        parser->get_func()->process(lgr) ;
                    }
                    return true ;
                }
            }
            else if(!parser_parent) { //reject
                break ;
            }

            if((parser_parent && parser_parent->is_rejected_with_ready()) || 
               (parser && parser->is_rejected_with_ready())) {

                ff.undo_parsers(1, vmode) ;
                lgr.remove_from_back(1) ;
                parser_parent->undo_state(1) ;
                parent_lgr.remove_from_back(1) ;
            }
        }
        return false ;
    }
}


namespace vind
{
    //EdiCopyHighlightText (EdiVisual only)
    EdiCopyHighlightText::EdiCopyHighlightText()
    : BindedFuncCreator("edi_copy_highlight_text")
    {}
    void EdiCopyHighlightText::sprocess() {
        using namespace mode ;

        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;

        if(get_global_mode() == Mode::EdiLineVisual)
            g_rgtype = RegisteredType::Lines ;
        else
            g_rgtype = RegisteredType::Chars ;

        keybrd::pushup(KEYCODE_LEFT) ; //unselect, but this is for the time being
        Change2EdiNormal::sprocess(true) ;
    }
    void EdiCopyHighlightText::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EdiCopyHighlightText::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EdiNCopyLine (EdiNormal only)
    EdiNCopyLine::EdiNCopyLine()
    : BindedFuncCreator("edi_n_copy_line")
    {}
    void EdiNCopyLine::sprocess(
            unsigned int repeat_num,
            const textanalyze::SelRes* const exres) {

        using keybrd::pushup ;
        pushup(KEYCODE_HOME) ;

        //copy N - 1 lines
        for(unsigned int i = 1 ; i < repeat_num ; i ++)
            pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;

        if(!select_line_until_EOL(exres))
            clear_clipboard_with_null() ;

        pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        pushup(KEYCODE_END) ;

        g_rgtype = RegisteredType::Lines ;
    }
    void EdiNCopyLine::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiNCopyLine::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1, nullptr) ;
    }


    //EdiCopyMotion
    struct EdiCopyMotion::Impl {
        FuncFinder funcfinder_ ;
        FuncFinder parent_funcfinder_ ;

        explicit Impl()
        : funcfinder_(),
          parent_funcfinder_()
        {}

        void copy() const {
            EdiCopyHighlightText::sprocess() ;
            Change2EdiNormal::sprocess(false) ;
        }
    } ;
    EdiCopyMotion::EdiCopyMotion()
    : BindedFuncCreator("edi_copy_motion"),
      pimpl(std::make_unique<Impl>())
    {}
    EdiCopyMotion::~EdiCopyMotion() noexcept                 = default ;
    EdiCopyMotion::EdiCopyMotion(EdiCopyMotion&&)            = default ;
    EdiCopyMotion& EdiCopyMotion::operator=(EdiCopyMotion&&) = default ;
    void EdiCopyMotion::sprocess() const {
        if(select_by_motion(id(), pimpl->funcfinder_)) {
            pimpl->copy() ;
        }
    }
    void EdiCopyMotion::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            if(select_by_motion(id(), pimpl->funcfinder_,
                        pimpl->parent_funcfinder_, parent_lgr)) {
                pimpl->copy() ;
            }
        }
    }
    void EdiCopyMotion::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EdiCopyMotion::load_config() {
        pimpl->funcfinder_.reconstruct_funcset() ;
        pimpl->parent_funcfinder_.reconstruct_funcset() ;
    }
}


namespace vind
{
    //EdiNPasteAfter (EdiNormal or EdiVisual)
    struct EdiNPasteAfter::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNPasteAfter::EdiNPasteAfter()
    : BindedFuncCreator("edi_n_paste_after"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNPasteAfter::~EdiNPasteAfter() noexcept                  = default ;
    EdiNPasteAfter::EdiNPasteAfter(EdiNPasteAfter&&)            = default ;
    EdiNPasteAfter& EdiNPasteAfter::operator=(EdiNPasteAfter&&) = default ;

    void EdiNPasteAfter::sprocess(unsigned int repeat_num) const {
        using keybrd::pushup ;
        if(g_rgtype == RegisteredType::Chars) {
            pushup(KEYCODE_RIGHT) ;
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                pushup(KEYCODE_LCTRL, KEYCODE_V) ;
            }
        }
        else if(g_rgtype == RegisteredType::Lines) {
            pushup(KEYCODE_END) ;
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                pushup(KEYCODE_ENTER) ;
                pushup(KEYCODE_LCTRL, KEYCODE_V) ;
            }
        }
    }
    void EdiNPasteAfter::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNPasteAfter::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiNPasteBefore
    struct EdiNPasteBefore::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNPasteBefore::EdiNPasteBefore()
    : BindedFuncCreator("edi_n_paste_before"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNPasteBefore::~EdiNPasteBefore() noexcept                   = default ;
    EdiNPasteBefore::EdiNPasteBefore(EdiNPasteBefore&&)            = default ;
    EdiNPasteBefore& EdiNPasteBefore::operator=(EdiNPasteBefore&&) = default ;

    void EdiNPasteBefore::sprocess(unsigned int repeat_num) const {
        using keybrd::pushup ;
        if(g_rgtype == RegisteredType::Chars) {
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                pushup(KEYCODE_LCTRL, KEYCODE_V) ;
            }
        }
        else if(g_rgtype == RegisteredType::Lines) {
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                pushup(KEYCODE_HOME) ;
                pushup(KEYCODE_ENTER) ;
                pushup(KEYCODE_UP) ;
                pushup(KEYCODE_LCTRL, KEYCODE_V) ;
            }
        }
    }
    void EdiNPasteBefore::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNPasteBefore::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiDeleteHighlightText (Visual only)
    EdiDeleteHighlightText::EdiDeleteHighlightText()
    : BindedFuncCreator("edi_delete_highlight_text")
    {}
    void EdiDeleteHighlightText::sprocess() {
        using namespace mode ;
        using keybrd::pushup ;

        pushup(KEYCODE_LCTRL, KEYCODE_X) ;
        if(get_global_mode() == Mode::EdiLineVisual) {
            g_rgtype = RegisteredType::Lines ;
        }
        else {
            g_rgtype = RegisteredType::Chars ;
        }
        Change2EdiNormal::sprocess(false) ;
    }
    void EdiDeleteHighlightText::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EdiDeleteHighlightText::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}


namespace
{
    inline void delete_line_when_selecting() {
        using namespace mode ;
        const auto mode = get_global_mode() ;
        if(mode == Mode::EdiVisual) {
            if(iparams::get_b("enable_char_cache")) {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                g_rgtype = RegisteredType::Chars ;
            }
            else {
                keybrd::pushup(KEYCODE_DELETE) ;
            }
        }
        else if(mode == Mode::EdiLineVisual) {
            if(iparams::get_b("enable_char_cache")) {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            }
            else {
                keybrd::pushup(KEYCODE_DELETE) ;
            }
            keybrd::pushup(KEYCODE_DELETE) ;
            g_rgtype = RegisteredType::Lines ;
        }
    }
}

namespace vind
{
    //EdiNDeleteLine
    struct EdiNDeleteLine::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteLine::EdiNDeleteLine()
    : BindedFuncCreator("edi_n_delete_line"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteLine::~EdiNDeleteLine() noexcept                  = default ;
    EdiNDeleteLine::EdiNDeleteLine(EdiNDeleteLine&&)            = default ;
    EdiNDeleteLine& EdiNDeleteLine::operator=(EdiNDeleteLine&&) = default ;

    void EdiNDeleteLine::sprocess(unsigned int repeat_num) const {
        using keybrd::pushup ;
        pushup(KEYCODE_HOME) ;

        for(decltype(repeat_num) i = 1 ; i < repeat_num ; i ++) {
            pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
        }

        if(!select_line_until_EOL(nullptr)) {
            clear_clipboard_with_null() ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            g_rgtype = RegisteredType::Lines ;
            pushup(KEYCODE_DELETE) ;
        }
    }
    void EdiNDeleteLine::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNDeleteLine::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiNDeleteLineUntilEOL
    struct EdiNDeleteLineUntilEOL::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL()
    : BindedFuncCreator("edi_n_delete_line_until_EOL"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteLineUntilEOL::~EdiNDeleteLineUntilEOL() noexcept                          = default ;
    EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL(EdiNDeleteLineUntilEOL&&)            = default ;
    EdiNDeleteLineUntilEOL& EdiNDeleteLineUntilEOL::operator=(EdiNDeleteLineUntilEOL&&) = default ;

    void EdiNDeleteLineUntilEOL::sprocess(unsigned int repeat_num) const {
        using keybrd::pushup ;

        //delete N - 1 lines under the current line
        for(decltype(repeat_num) i = 1 ; i < repeat_num ; i ++) {
            pushup(KEYCODE_DOWN) ;
        }

        if(!select_line_until_EOL(nullptr)) {
            clear_clipboard_with_null() ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            g_rgtype = RegisteredType::Chars ;
        }
    }
    void EdiNDeleteLineUntilEOL::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNDeleteLineUntilEOL::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiNDeleteAfter
    struct EdiNDeleteAfter::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteAfter::EdiNDeleteAfter()
    : BindedFuncCreator("edi_n_delete_after"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteAfter::~EdiNDeleteAfter() noexcept                   = default ;
    EdiNDeleteAfter::EdiNDeleteAfter(EdiNDeleteAfter&&)            = default ;
    EdiNDeleteAfter& EdiNDeleteAfter::operator=(EdiNDeleteAfter&&) = default ;

    void EdiNDeleteAfter::sprocess(unsigned int repeat_num) const {
        if(iparams::get_b("enable_char_cache")) {
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                    keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                    keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                    g_rgtype = RegisteredType::Chars ;
            }
        }
        else {
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                keybrd::pushup(KEYCODE_DELETE) ;
            }
        }
    }
    void EdiNDeleteAfter::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNDeleteAfter::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiNDeleteBefore
    struct EdiNDeleteBefore::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteBefore::EdiNDeleteBefore()
    : BindedFuncCreator("edi_n_delete_before"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteBefore::~EdiNDeleteBefore() noexcept                    = default ;
    EdiNDeleteBefore::EdiNDeleteBefore(EdiNDeleteBefore&&)            = default ;
    EdiNDeleteBefore& EdiNDeleteBefore::operator=(EdiNDeleteBefore&&) = default ;

    void EdiNDeleteBefore::sprocess(unsigned int repeat_num) const {
        if(iparams::get_b("enable_char_cache")) {
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                g_rgtype = RegisteredType::Chars ;
            }
        }
        else {
            for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
                keybrd::pushup(KEYCODE_BKSPACE) ;
            }
        }
    }
    void EdiNDeleteBefore::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNDeleteBefore::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //EdiDeleteMotion
    struct EdiDeleteMotion::Impl {
        FuncFinder funcfinder_ ;
        FuncFinder parent_funcfinder_ ;

        explicit Impl()
        : funcfinder_(),
          parent_funcfinder_()
        {}

        void remove() const {
            EdiDeleteHighlightText::sprocess() ;
        }
    } ;
    EdiDeleteMotion::EdiDeleteMotion()
    : BindedFuncCreator("edi_delete_motion"),
      pimpl(std::make_unique<Impl>())
    {}
    EdiDeleteMotion::~EdiDeleteMotion() noexcept                   = default ;
    EdiDeleteMotion::EdiDeleteMotion(EdiDeleteMotion&&)            = default ;
    EdiDeleteMotion& EdiDeleteMotion::operator=(EdiDeleteMotion&&) = default ;

    void EdiDeleteMotion::sprocess() const {
        if(select_by_motion(id(), pimpl->funcfinder_)) {
            pimpl->remove() ;
        }
    }
    void EdiDeleteMotion::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            if(select_by_motion(id(), pimpl->funcfinder_,
                        pimpl->funcfinder_, parent_lgr)) {
                pimpl->remove() ;
            }
        }
    }
    void EdiDeleteMotion::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EdiDeleteMotion::load_config() {
        pimpl->funcfinder_.reconstruct_funcset() ;
        pimpl->parent_funcfinder_.reconstruct_funcset() ;
    }


    //EdiDeleteMotionAndStartInsert
    struct EdiDeleteMotionAndStartInsert::Impl {
        FuncFinder funcfinder_ ;
        FuncFinder parent_funcfinder_ ;

        explicit Impl()
        : funcfinder_(),
          parent_funcfinder_()
        {}

        void remove_and_insert() {
            EdiDeleteHighlightText::sprocess() ;
            Change2EdiInsert::sprocess(false) ;
        }
    } ;
    EdiDeleteMotionAndStartInsert::EdiDeleteMotionAndStartInsert()
    : BindedFuncCreator("edi_delete_motion_and_start_insert"),
      pimpl(std::make_unique<Impl>())
    {}
    EdiDeleteMotionAndStartInsert::~EdiDeleteMotionAndStartInsert() noexcept                                 = default ;
    EdiDeleteMotionAndStartInsert::EdiDeleteMotionAndStartInsert(EdiDeleteMotionAndStartInsert&&)            = default ;
    EdiDeleteMotionAndStartInsert& EdiDeleteMotionAndStartInsert::operator=(EdiDeleteMotionAndStartInsert&&) = default ;
    void EdiDeleteMotionAndStartInsert::sprocess() const {
        if(select_by_motion(id(), pimpl->funcfinder_)) {
            pimpl->remove_and_insert() ;
        }
    }
    void EdiDeleteMotionAndStartInsert::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            if(select_by_motion(id(), pimpl->funcfinder_,
                        pimpl->parent_funcfinder_, parent_lgr)) {
                pimpl->remove_and_insert() ;
            }
        }
    }
    void EdiDeleteMotionAndStartInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EdiDeleteMotionAndStartInsert::load_config() {
        pimpl->funcfinder_.reconstruct_funcset() ;
        pimpl->parent_funcfinder_.reconstruct_funcset() ;
    }


    //EdiDeleteLinesAndStartInsert
    EdiDeleteLinesAndStartInsert::EdiDeleteLinesAndStartInsert()
    : BindedFuncCreator("edi_delete_lines_and_start_insert")
    {}
    void EdiDeleteLinesAndStartInsert::sprocess(unsigned int repeat_num) {
        auto res = textanalyze::get_selected_text([] {
            keybrd::pushup(KEYCODE_HOME) ;
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        }) ;
        if(res.str.empty()) {
            Change2EdiInsert::sprocess(false) ;
            return ;
        }

        const auto pos = res.str.find_first_not_of(" \t") ; //position except for space or tab
        if(pos == std::string::npos) { //space only
            Change2EdiEOLInsert::sprocess(false) ;
            return ;
        }
        keybrd::pushup(KEYCODE_HOME) ;

        for(int i = 0 ; i < static_cast<int>(pos) ; i ++) {
            keybrd::pushup(KEYCODE_RIGHT) ;
        }
        EdiDeleteUntilEOLAndStartInsert::sprocess(repeat_num, &res) ;
    }
    void EdiDeleteLinesAndStartInsert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiDeleteLinesAndStartInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //EdiDeleteCharsAndStartInsert
    EdiDeleteCharsAndStartInsert::EdiDeleteCharsAndStartInsert()
    : BindedFuncCreator("edi_delete_chars_and_start_insert")
    {}
    void EdiDeleteCharsAndStartInsert::sprocess(unsigned int repeat_num) {
        for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
        }

        if(iparams::get_b("enable_char_cache")) {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            g_rgtype = RegisteredType::Chars ;
        }
        else {
            keybrd::pushup(KEYCODE_DELETE) ;
        }

        Change2EdiInsert::sprocess(false) ;
    }
    void EdiDeleteCharsAndStartInsert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiDeleteCharsAndStartInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //EdiDeleteUntilEOLAndStartInsert
    EdiDeleteUntilEOLAndStartInsert::EdiDeleteUntilEOLAndStartInsert()
    : BindedFuncCreator("edi_delete_until_eol_and_start_insert")
    {}
    /* Actually, If N >= 2
     *
     * Command: 2C
     * 
     * If the caret is positioned the head of a line.
     *
     * Original Vim:
     * [Before]
     *      |   AAAAAA
     *          BBBBBB
     *          CCCCCC
     * [After]
     *      |
     *          CCCCC
     *
     * win-vind:
     * [Before]
     *      |   AAAAAA
     *          BBBBBB
     *          CCCCCC
     * [After]
     *         |CCCCCC
     *
     * In future, must fix.
     *
     */
    void EdiDeleteUntilEOLAndStartInsert::sprocess(
            unsigned int repeat_num,
            const textanalyze::SelRes* const exres) {

        for(unsigned int i = 1 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;
        }

        if(!select_line_until_EOL(exres)) {
            clear_clipboard_with_null() ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            g_rgtype = RegisteredType::Chars ;
        }
        Change2EdiInsert::sprocess(false) ;
    }
    void EdiDeleteUntilEOLAndStartInsert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiDeleteUntilEOLAndStartInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }
}
