#include "key_binder.hpp"

#include <algorithm>
#include <iostream>
#include <mutex>
#include <deque>

#include <windows.h>

#include "xml_parser.hpp"
#include "ini_parser.hpp"
#include "mode_manager.hpp"
#include "key_logger.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"
#include "msg_logger.hpp"
#include "dynamic_config.hpp"

//funcs
#include "move_cursor.hpp"
#include "jump_cursor.hpp"
#include "scroll.hpp"
#include "click.hpp"
#include "change_mode.hpp"
#include "undo.hpp"
#include "clipboard.hpp"
#include "switch_vdesktop.hpp"
#include "search_pattern.hpp"
#include "select.hpp"

//editor
#include "edi_change_mode.hpp"
#include "edi_move_caret.hpp"
#include "edi_jump_caret.hpp"
#include "edi_clipboard.hpp"
#include "edi_replace.hpp"
#include "edi_layout.hpp"

//cmd?
#include "window_ctrl.hpp"
#include "pager.hpp"
#include "filer.hpp"
#include "external_app.hpp"
#include "mywindow_ctrl.hpp"

using namespace std ;

struct KeyBinder::Impl
{
    vector<bf::shp_t> vpbf_normal {
        Change2Normal::create_with_cache(),
        Change2Insert::create(),
        Change2Visual::create(),
        Change2Editor::create(),
        Change2Command::create_with_cache(),

        SelectAll::create(),

        MoveLeft::create_with_cache(),
        MoveRight::create_with_cache(),
        MoveUp::create_with_cache(),
        MoveDown::create_with_cache(),

        Jump2Left::create_with_cache(),
        Jump2Right::create_with_cache(),
        Jump2Top::create_with_cache(),
        Jump2Bottom::create_with_cache(),
        Jump2XCenter::create_with_cache(),
        Jump2YCenter::create_with_cache(),
        Jump2Any::create_with_cache(),
        Jump2ActiveWindow::create_with_cache(),

        ScrollLeft::create_with_cache(),
        ScrollRight::create_with_cache(),
        ScrollUp::create_with_cache(),
        ScrollDown::create_with_cache(),
        ScrollMidLeft::create_with_cache(),
        ScrollMidRight::create_with_cache(),
        ScrollMidUp::create_with_cache(),
        ScrollMidDown::create_with_cache(),
        ScrollPageUp::create_with_cache(),
        ScrollPageDown::create_with_cache(),

        ClickLeft::create_with_cache(),
        ClickRight::create_with_cache(),

        CBCopy::create_with_cache(),
        CBPaste::create_with_cache(),
        CBCut::create_with_cache(),
        CBDelete::create_with_cache(),
        CBBackSpace::create_with_cache(),

        SCRedo::create_with_cache(),
        SCUndo::create_with_cache(),

        SwitchVDesktop2Left::create(),
        SwitchVDesktop2Right::create(),

        SearchPattern::create(),

        CloseCurrentWindow::BindedFunctionWithCreator::create(),
        SwitchWindow::BindedFunctionWithCreator::create(),
        MaximizeCurrentWindow::BindedFunctionWithCreator::create(),
        MinimizeCurrentWindow::BindedFunctionWithCreator::create(),
        SnapCurrentWindow2Left::BindedFunctionWithCreator::create(),
        SnapCurrentWindow2Right::BindedFunctionWithCreator::create(),
        Move2NextPage::create(),
        Move2PrevPage::create(),

        OpenNewCurrentWindow::BindedFunctionWithCreator::create()
    } ;

    vector<bf::shp_t> vpbf_insert {
        Change2Normal::create_with_cache()
    } ;

    vector<bf::shp_t> vpbf_visual {
        Change2Normal::create_with_cache(),

        MoveLeft::create_with_cache(),
        MoveRight::create_with_cache(),
        MoveUp::create_with_cache(),
        MoveDown::create_with_cache(),

        Jump2Left::create_with_cache(),
        Jump2Right::create_with_cache(),
        Jump2Top::create_with_cache(),
        Jump2Bottom::create_with_cache(),
        Jump2XCenter::create_with_cache(),
        Jump2YCenter::create_with_cache(),
        Jump2Any::create_with_cache(),
        Jump2ActiveWindow::create_with_cache(),

        ScrollLeft::create_with_cache(),
        ScrollRight::create_with_cache(),
        ScrollUp::create_with_cache(),
        ScrollDown::create_with_cache(),
        ScrollMidLeft::create_with_cache(),
        ScrollMidRight::create_with_cache(),
        ScrollMidUp::create_with_cache(),
        ScrollMidDown::create_with_cache(),
        ScrollPageUp::create_with_cache(),
        ScrollPageDown::create_with_cache(),

        ClickLeft::create_with_cache(),
        ClickRight::create_with_cache(),

        CBCopy::create_with_cache(),
        CBPaste::create_with_cache(),
        CBCut::create_with_cache(),
        CBDelete::create_with_cache(),
        CBBackSpace::create_with_cache()
    } ;

    vector<bf::shp_t> vpbf_edi_normal {
        Change2Normal::create_with_cache(),
        Change2Command::create_with_cache(),
        Change2EdiInsert::create(),
        Change2EdiBOLInsert::create(),
        Change2EdiBkInsert::create(),
        Change2EdiEOLInsert::create(),
        Change2EdiNlInsertBelow::create(),
        Change2EdiNlInsertAbove::create(),
        Change2EdiVisual::create(),
        Change2EdiLineVisual::create(),

        EdiNCopyLine::create(),
        EdiNPasteAfter::create_with_cache(),
        EdiNPasteBefore::create_with_cache(),
        EdiNDeleteLine::create_with_cache(),
        EdiNDeleteLineUntilEOL::create_with_cache(),
        EdiNDeleteAfter::create_with_cache(),
        EdiNDeleteBefore::create_with_cache(),

        EdiNRemoveEOL::create_with_cache(),

        EdiNReplaceChar::create(),
        EdiNReplaceSequence::create(),

        SCRedo::create_with_cache(),
        SCUndo::create_with_cache(),

        ScrollLeft::create_with_cache(),
        ScrollRight::create_with_cache(),
        ScrollUp::create_with_cache(),
        ScrollDown::create_with_cache(),
        ScrollMidLeft::create_with_cache(),
        ScrollMidRight::create_with_cache(),
        ScrollMidUp::create_with_cache(),
        ScrollMidDown::create_with_cache(),
        ScrollPageUp::create_with_cache(),
        ScrollPageDown::create_with_cache(),

        EdiMoveCaretLeft::create_with_cache(),
        EdiMoveCaretRight::create_with_cache(),
        EdiMoveCaretUp::create_with_cache(),
        EdiMoveCaretDown::create_with_cache(),

        EdiNMoveCaretWORDSForward::create_with_cache(),
        EdiNMoveCaretWORDSBackward::create_with_cache(),
        EdiNMoveCaretwordsForward::create_with_cache(),
        EdiNMoveCaretwordsBackward::create_with_cache(),

        EdiJumpCaret2BOL::create_with_cache(),
        EdiJumpCaret2EOL::create_with_cache(),
        EdiNJumpCaret2Line_DfBOF::create_with_cache(),
        EdiNJumpCaret2Line_DfEOF::create_with_cache()
    } ;

    vector<bf::shp_t> vpbf_edi_insert {
        Change2Normal::create_with_cache(),
        Change2EdiNormal::create_with_cache()
    } ;

    vector<bf::shp_t> vpbf_edi_visual {
        Change2Normal::create_with_cache(),
        Change2EdiNormal::create_with_cache(),

        EdiCopyHighlightText::create(),

        EdiNPasteAfter::create_with_cache(),
        EdiNPasteBefore::create_with_cache(),

        EdiDeleteHighlightText::create(),
        EdiNDeleteLineUntilEOL::create_with_cache(),
        EdiNDeleteAfter::create_with_cache(),
        EdiNDeleteBefore::create_with_cache(),

        EdiNRemoveEOL::create_with_cache(),

        SCRedo::create_with_cache(),
        SCUndo::create_with_cache(),

        ScrollLeft::create_with_cache(),
        ScrollRight::create_with_cache(),
        ScrollUp::create_with_cache(),
        ScrollDown::create_with_cache(),
        ScrollMidLeft::create_with_cache(),
        ScrollMidRight::create_with_cache(),
        ScrollMidUp::create_with_cache(),
        ScrollMidDown::create_with_cache(),
        ScrollPageUp::create_with_cache(),
        ScrollPageDown::create_with_cache(),

        EdiMoveCaretLeft::create_with_cache(),
        EdiMoveCaretRight::create_with_cache(),
        EdiMoveCaretUp::create_with_cache(),
        EdiMoveCaretDown::create_with_cache(),

        EdiNMoveCaretWORDSForward::create_with_cache(),
        EdiNMoveCaretWORDSBackward::create_with_cache(),
        EdiNMoveCaretwordsForward::create_with_cache(),
        EdiNMoveCaretwordsBackward::create_with_cache(),

        EdiJumpCaret2BOL::create_with_cache(),
        EdiJumpCaret2EOL::create_with_cache(),
        EdiNJumpCaret2Line_DfBOF::create_with_cache(),
        EdiNJumpCaret2Line_DfEOF::create_with_cache()
    } ;

    //Window Mode Command
    vector<cmd::shp_t> vpcmd {
        SaveOpenedFile::create_with_cache(),
        CloseOpenedFile::create_with_cache(),
        CloseCurrentWindow::CommandWithCreator::create_with_cache(),
        SwitchWindow::CommandWithCreator::create(),
        MaximizeCurrentWindow::CommandWithCreator::create(),
        MinimizeCurrentWindow::CommandWithCreator::create(),
        SnapCurrentWindow2Left::CommandWithCreator::create(),
        SnapCurrentWindow2Right::CommandWithCreator::create(),
        OpenNewCurrentWindow::CommandWithCreator::create(),
        ReloadCurrentWindow::create(),
        StartShell::create(),
        StartAnyApp::create(),
        ShowConfigWindow::create(),
        ExitConfigWindow::create_with_cache(),
        OpenOtherFile::create_with_cache(),
        MakeDir::create()
    } ;

    //Editor Mode Command
    vector<cmd::shp_t> vpcmd_edi {
        SaveOpenedFile::create_with_cache(),
        CloseOpenedFile::create_with_cache(),

        CloseCurrentWindow::CommandWithCreator::create_with_cache(),

        ExitConfigWindow::create_with_cache(),
        OpenOtherFile::create_with_cache()
    } ;

    KeyLogger logger{} ;

    bf::shp_t callable_bf{nullptr} ;

    std::size_t cmd_hist_index = 0 ;
    struct CmdPoint {
        std::unique_ptr<KeyLogger> logger ;
        cmd::shp_t  func ;

        explicit CmdPoint()
        : logger(std::make_unique<KeyLogger>()), func(nullptr)
        {}
    } ;
    std::deque<std::shared_ptr<CmdPoint>> cmd_hist{std::make_shared<CmdPoint>()} ;

    void update_history() {
        if(cmd_hist_index == cmd_hist.size() - 1) {
            //recently logger
            while(cmd_hist.size() >= DynamicConfig::CMD_MAX_HISTORY_NUM()) {
                cmd_hist.pop_front() ;
            }

            cmd_hist_index = cmd_hist.size() ; //update to index of recently history
            cmd_hist.emplace_back(std::make_shared<CmdPoint>()) ;
            return ;
        }

        //past logger
        cmd_hist_index = cmd_hist.size() - 1 ;
    }

    explicit Impl() {}

    ~Impl()
    {
        vpbf_normal.clear() ;
        vpbf_insert.clear() ;
        vpbf_visual.clear() ;
        vpbf_edi_normal.clear() ;
        vpbf_edi_insert.clear() ;
        vpbf_edi_visual.clear() ;
        vpcmd.clear() ;
        vpcmd_edi.clear() ;
    }

    Impl(Impl&&) = delete ;
    Impl& operator=(Impl&&) = delete ;
    Impl(const Impl&) = delete ;
    Impl& operator=(const Impl&) = delete ;
} ;

KeyBinder::KeyBinder()
: pimpl(make_unique<Impl>())
{}

KeyBinder::~KeyBinder() noexcept = default ;

KeyBinder::KeyBinder(const string& filename)
: KeyBinder()
{
    load_config(filename) ;
}

void KeyBinder::load_config(const string& filename) noexcept
{
    //initialize key list ------------------------------------------
    const auto map = XMLParser::load_bind_map(filename) ;
    const auto set = [](const auto& bindmap, auto& vpbf) {
        for(auto& i : vpbf) {
            try {i->set_command(bindmap.at(i->name())) ;}
            catch(out_of_range&) {continue ;}
        }
    } ;

    set(map, pimpl->vpbf_normal) ;
    set(map, pimpl->vpbf_insert) ;
    set(map, pimpl->vpbf_visual) ;

    set(map, pimpl->vpbf_edi_normal) ;
    set(map, pimpl->vpbf_edi_insert) ;
    set(map, pimpl->vpbf_edi_visual) ;

    //load commands ---------------------------------------------------
    const auto cmd_map = XMLParser::load_command_map(filename) ;
    set(cmd_map, pimpl->vpcmd) ;
    set(cmd_map, pimpl->vpcmd_edi) ;

    ExAppUtility::load_config() ;
    DynamicConfig::load_config() ;

    MESSAGE_STREAM << "Loaded " << filename << endl ;
}

namespace KBUtility{
    inline static auto is_ignored(const std::vector<unsigned char>& ignore_list, const KeyLog& log) noexcept {
        return std::all_of(log.cbegin(), log.cend(), [&ignore_list](const auto& key) {
            return std::find(ignore_list.cbegin(), ignore_list.cend(), key) != ignore_list.cend() ;
        }) ;
    }

    inline static void refresh_display() noexcept {
        if(!InvalidateRect(NULL, NULL, TRUE)) {
            ERROR_STREAM << "windows.h: " << GetLastError() << " failed refresh display (KeyBinder::InvalidateRect)\n" ;
            return ;
        }
    }
}

void KeyBinder::update_core(const vector<bf::shp_t>& vp) noexcept
{
    static const auto& ignore_alone = XMLParser::get_ignore_alone() ;
    using namespace KBUtility ;

    if(!pimpl->logger.is_changed_and_update()) {
        if(!pimpl->callable_bf) {
            pimpl->logger.remove_from_back(1) ;
            return ;
        }
        pimpl->callable_bf->process(false) ;
        pimpl->logger.remove_from_back(1) ;
        return ;
    }

    if(is_ignored(ignore_alone, pimpl->logger.back())) {
        //all is ignore code
        pimpl->logger.remove_from_back(1) ;
        pimpl->callable_bf = nullptr ;
        return ;
    }

    //typed key is changed.---------------------------------------------

    auto at_least_exist = false ; //is typed key existed in binded functions?
    size_t max_matching_num = 0 ;

    bf::shp_t buf_bf = pimpl->callable_bf ;

    //overwrite callable
    for(auto& func : vp) {
        const auto lmn = func->is_matching(pimpl->logger.back(), pimpl->logger.size() - 1) ;
        if(lmn == 0) {
            continue ;
        }

        at_least_exist = true ;

        if(func->is_callable()) {
            if(pimpl->callable_bf == func) {
                continue ;
            }
            if(max_matching_num >= lmn) {
                continue ;
            }

            max_matching_num = lmn ;
            buf_bf = func ;
        }
    }

    if(!at_least_exist) {
        pimpl->logger.clear() ;
        pimpl->callable_bf = nullptr ;
        return ;
    }

    if(!buf_bf) {
        pimpl->callable_bf = nullptr ;
        return ;
    }

    buf_bf->process(true) ;
    pimpl->logger.clear() ;
    pimpl->callable_bf = buf_bf ;
}

void KeyBinder::update_core_cmd(const std::vector<cmd::shp_t>& vp) noexcept
{
    auto return_mode = [] {
        const auto mode = ModeManager::get_mode() ;
        using ModeManager::Mode ;
        using ModeManager::change_mode ;
        if(mode == Mode::EdiCommand) {
            change_mode(Mode::EdiNormal) ;
        }
        else {
            change_mode(Mode::Normal) ;
        }
    } ;

    static const auto ignore_alone = VKCConverter::get_all_sys_vkc() ;
    using namespace KBUtility ;

    auto& p_cmdp = pimpl->cmd_hist.at(pimpl->cmd_hist_index) ;
    auto& plger = p_cmdp->logger ;

    if(!plger->is_changed_and_inputc()) {
        plger->remove_from_back(1) ;
        return ;
    }

    //empty input is skipped
    if(plger->back().is_empty()) {
        plger->remove_from_back(1) ;
        return ;
    }

    //breaking
    if(plger->back().is_including(VKC_ESC)){
        const auto recent_index = pimpl->cmd_hist.size() - 1 ;
        if(pimpl->cmd_hist_index == recent_index) {
            plger->clear() ;
            p_cmdp->func = nullptr ;
        }
        else {
            plger->remove_from_back(1) ;
            pimpl->cmd_hist_index = recent_index ;
        }
        return_mode() ;
        refresh_display() ;
        return ;
    }

    if(plger->back().is_including(VKC_ENTER) && p_cmdp->func) {
        plger->remove_from_back(1) ; //remove keycode of enter
        p_cmdp->func->process(plger->get_str()) ;

        //plger->clear() ;
        //p_cmdp->func = nullptr ;

        pimpl->update_history() ;

        return_mode() ;
        refresh_display() ;
        return ;
    }

    //edit command
    if(plger->back().is_including(VKC_BKSPACE)) {
        plger->remove_from_back(2) ;
        refresh_display() ;

        if(plger->is_empty()) {
            p_cmdp->func = nullptr ;
            return ;
        }
    }

    //operate command history
    if(plger->back().is_including(VKC_UP) && pimpl->cmd_hist_index > 0) {
        pimpl->cmd_hist_index -- ;
        plger->remove_from_back(1) ;
        refresh_display() ;
        return ;
    }
    if(plger->back().is_including(VKC_DOWN) && pimpl->cmd_hist_index < pimpl->cmd_hist.size() - 1) {
        pimpl->cmd_hist_index ++ ;
        plger->remove_from_back(1) ;
        refresh_display() ;
        return  ;
    }

    //invalid keys
    if(is_ignored(ignore_alone, plger->back()) || plger->size() > DynamicConfig::CMD_MAX_CHAR()) {
        plger->remove_from_back(1) ;
        return ;
    }

    //search cmd from cmd list
    for(auto& func : vp) {
        if(func->is_callable(*plger)) {
            p_cmdp->func = func ;
            return ;
        }
    }

    p_cmdp->func = nullptr ;
}

void KeyBinder::update() noexcept {
    using namespace ModeManager ;
    switch(get_mode()) {
        case Mode::Normal:
            update_core(pimpl->vpbf_normal) ;
            break ;

        case Mode::Insert:
            update_core(pimpl->vpbf_insert) ;
            break ;

        case Mode::Visual:
            update_core(pimpl->vpbf_visual) ;
            break ;

        case Mode::Command:
            update_core_cmd(pimpl->vpcmd) ;
            break ;

        case Mode::EdiNormal:
            update_core(pimpl->vpbf_edi_normal) ;
            break ;

        case Mode::EdiInsert:
            update_core(pimpl->vpbf_edi_insert) ;
            break ;

        case Mode::EdiVisual:
            update_core(pimpl->vpbf_edi_visual) ;
            break ;

        case Mode::EdiLineVisual:
            update_core(pimpl->vpbf_edi_visual) ;
            break ;

        case Mode::EdiCommand:
            update_core_cmd(pimpl->vpcmd_edi) ;
            break ;

        default:
            change_mode(Mode::Normal) ;
            break ;
    }
    //post process
}

const string KeyBinder::get_logger_str() const noexcept
{
    try {
        return pimpl->cmd_hist.at(pimpl->cmd_hist_index)->logger->get_str() ;
    }
    catch(std::out_of_range& e) {
        return string() ;
    }
}