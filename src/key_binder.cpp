#include "key_binder.hpp"
#include "xml_parser.hpp"
#include "ini_parser.hpp"
#include "mode_manager.hpp"
#include "key_logger.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"
#include "msg_logger.hpp"
#include "default_config.hpp"

//funcs
#include "move_cursor.hpp"
#include "jump_cursor.hpp"
#include "scroll.hpp"
#include "click.hpp"
#include "change_mode.hpp"
#include "undo.hpp"
#include "clipboard.hpp"
#include "switch_taskview.hpp"
#include "search_pattern.hpp"
#include "select.hpp"

//editor
#include "edi_move_cursor.hpp"
#include "edi_change_mode.hpp"

//cmd?
#include "window_ctrl.hpp"
#include "pager.hpp"
#include "filer.hpp"
#include "external_app.hpp"

#include <algorithm>
#include <iostream>
#include <mutex>

#include <windows.h>

using namespace std ;

struct KeyBinder::Impl
{
    vector<bf::shp_t> vpbf_normal {
        Change2Normal::create_with_cache(),
        Change2Insert::create(),
        Change2Visual::create(),
        //Change2Editor::create(),
        Change2Command::create(),

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
        
        SCRedo::create(),
        SCUndo::create(),

        SwitchTaskview2Left::create(),
        SwitchTaskview2Right::create(),

        SearchPattern::create(),

        CloseCurrentWindow::BindedFunctionWithCreator::create(),
        SwitchWindow::BindedFunctionWithCreator::create(),
        MaximizeCurrentWindow::BindedFunctionWithCreator::create(),
        MinimizeCurrentWindow::BindedFunctionWithCreator::create(),
        SnapCurrentWindow2Left::BindedFunctionWithCreator::create(),
        SnapCurrentWindow2Right::BindedFunctionWithCreator::create(),
        Move2NextPage::create(),
        Move2PrevPage::create()
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
        CBDelete::create_with_cache()
    } ;

    vector<bf::shp_t> vpbf_edi_normal {
        Change2Normal::create_with_cache(),
        Change2EdiInsert::create(),
        Change2EdiBkInsert::create(),
        Change2EdiNlInsert::create(),
        Change2EdiVisual::create(),
        
        EdiMoveLeft::create_with_cache(),
        EdiMoveRight::create_with_cache(),
        EdiMoveUp::create_with_cache(),
        EdiMoveDown::create_with_cache()
    } ;

    vector<bf::shp_t> vpbf_edi_insert {
        Change2Normal::create_with_cache(),
        Change2EdiNormal::create_with_cache()
    } ;

    vector<bf::shp_t> vpbf_edi_visual {
        Change2EdiNormal::create_with_cache(),

        EdiMoveLeft::create_with_cache(),
        EdiMoveRight::create_with_cache(),
        EdiMoveUp::create_with_cache(),
        EdiMoveDown::create_with_cache()
    } ;

    vector<cmd::shp_t> vpcmd {
        SaveOpenedFile::create(),
        CloseOpenedFile::create(),
        CloseCurrentWindow::CommandWithCreator::create(),
        SwitchWindow::CommandWithCreator::create(),
        MaximizeCurrentWindow::CommandWithCreator::create(),
        MinimizeCurrentWindow::CommandWithCreator::create(),
        SnapCurrentWindow2Left::CommandWithCreator::create(),
        SnapCurrentWindow2Right::CommandWithCreator::create(),
        StartShell::create(),
        StartAnyApp::create()
    } ;

    KeyLogger logger{} ;

    bf::shp_t callable_bf{nullptr} ;
    cmd::shp_t callable_cmd{nullptr} ;

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

    Logger::msg_stream << "[Message] Loaded " << filename << endl ;
}

namespace KBUtility{
    inline static auto is_ignored(const std::vector<unsigned char>& ignore_list, const KeyLog& log) noexcept {
        return std::all_of(log.cbegin(), log.cend(), [&ignore_list](const auto& key) {
            return std::find(ignore_list.cbegin(), ignore_list.cend(), key) != ignore_list.cend() ;
        }) ;
    }

    inline static void refresh_display() noexcept {
        if(!InvalidateRect(NULL, NULL, TRUE)) {
            Logger::msg_stream << "[Error] windows.h: " << GetLastError() << " failed refresh display (KeyBinder::InvalidateRect)\n" ;
            return ;
        }
    }
}

void KeyBinder::update_core(const vector<bf::shp_t>& vpbf) noexcept
{
    static const auto ignore_alone(XMLParser::get_ignore_alone()) ;
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
    for(auto& func : vpbf) {
        const auto lmn = func->existed_num_and_update(pimpl->logger.back(), pimpl->logger.size() - 1) ;
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

void KeyBinder::update_core_cmd() noexcept
{
    static const auto ignore_alone(VKCConverter::get_all_sys_vkc()) ;
    using namespace KBUtility ;


    if(!pimpl->logger.is_changed_and_inputc()) {
        pimpl->logger.remove_from_back(1) ;
        return ;
    }

    //empty input is skipped
    if(pimpl->logger.back().is_empty()) {
        pimpl->logger.remove_from_back(1) ;
        return ;
    }

    //breaking
    if(pimpl->logger.back().is_included(VKC_ESC)){
        pimpl->logger.clear() ;
        pimpl->callable_cmd = nullptr ;
        ModeManager::change_mode(ModeManager::Mode::Normal) ;
        refresh_display() ;
        return ;
    }

    if(pimpl->logger.back().is_included(VKC_ENTER) && pimpl->callable_cmd) {
        pimpl->logger.remove_from_back(1) ; //remove keycode of enter
        pimpl->callable_cmd->process(pimpl->logger.get_str()) ;

        pimpl->logger.clear() ;
        refresh_display() ;

        pimpl->callable_cmd = nullptr ;
        ModeManager::change_mode(ModeManager::Mode::Normal) ;
        return ;
    }

    //command edit
    if(pimpl->logger.back().is_included(VKC_BKSPACE)) {
        pimpl->logger.remove_from_back(2) ;
        refresh_display() ;

        if(pimpl->logger.is_empty()) {
            pimpl->callable_cmd = nullptr ;
            return ;
        }
    }

    //invalid keys
    if(is_ignored(ignore_alone, pimpl->logger.back()) || pimpl->logger.size() > DefaultConfig::CMD_MAX_CHAR()) {
        pimpl->logger.remove_from_back(1) ;
        return ;
    }

    //search cmd from cmd list
    for(auto& func : pimpl->vpcmd) {
        if(func->is_callable(pimpl->logger)) {
            pimpl->callable_cmd = func ;
            return ;
        }
    }

    pimpl->callable_cmd = nullptr ;
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

        case Mode::EdiNormal:
            update_core(pimpl->vpbf_edi_normal) ;
            break ;

        case Mode::EdiInsert:
            update_core(pimpl->vpbf_edi_insert) ;
            break ;

        case Mode::EdiVisual:
            update_core(pimpl->vpbf_edi_visual) ;
            break ;

        case Mode::Command:
            update_core_cmd() ;
            break ;

        default:
            change_mode(Mode::Normal) ;
            break ;
    }
    //post process
}

const string KeyBinder::get_logger_str() const noexcept
{
    return pimpl->logger.get_str() ;
}