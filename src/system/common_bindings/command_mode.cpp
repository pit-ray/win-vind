#include "command_mode.hpp"

#include <windows.h>

#include <vector>
#include <mutex>

#include "bindings_loader.hpp"
#include "i_params.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "system.hpp"
#include "utility.hpp"
#include "virtual_cmd_line.hpp"

namespace CmdModeUtility
{
    struct CmdPoint {
        KeyLogger logger{} ;
        BindedFunc::shp_t func = nullptr ;
    } ;
    static std::deque<std::shared_ptr<CmdPoint>> cmd_hist{std::make_shared<CmdPoint>()} ;
    static std::size_t cmd_hist_index = 0 ;

    inline void update_history() {
        if(cmd_hist_index == cmd_hist.size() - 1) {
            //recently logger
            while(cmd_hist.size() >= iParams::get_z("cmd_max_history_num")) {
                cmd_hist.pop_front() ;
            }

            cmd_hist_index = cmd_hist.size() ; //update to index of recently history
            cmd_hist.emplace_back(std::make_shared<CmdPoint>()) ;
            return ;
        }
        //past logger
        cmd_hist_index = cmd_hist.size() - 1 ;
    }
}

const std::string CommandMode::sname() noexcept
{
    return "command_mode" ;
}

//[true] continue, [false] terminate
bool update_core_cmd() noexcept {
    std::mutex mtx ;
    std::lock_guard<std::mutex> lock(mtx) ;

    using namespace CmdModeUtility ;

    auto& p_cmdp = cmd_hist.at(cmd_hist_index) ;
    auto& lgr = p_cmdp->logger ;

    if(!lgr.is_changed_char()) {
        lgr.remove_from_back(1) ;
        return true ;
    }

    if(lgr.back().is_containing(VKC_ESC)){
        const auto recent_index = cmd_hist.size() - 1 ;
        if(cmd_hist_index == recent_index) {
            lgr.clear() ;
            p_cmdp->func = nullptr ;
        }
        else {
            lgr.remove_from_back(1) ;
            cmd_hist_index = recent_index ;
        }
        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;
        return false ;
    }

    if(lgr.back().is_containing(VKC_ENTER) && p_cmdp->func) {
        lgr.remove_from_back(1) ; //remove keycode of enter

        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;

        p_cmdp->func->process(true, 1, &lgr) ;
        update_history() ;
        return false ;
    }

    //edit command
    if(lgr.back().is_containing(VKC_BKSPACE)) {
        lgr.remove_from_back(2) ;
        VirtualCmdLine::refresh() ;

        if(lgr.is_empty()) {
            p_cmdp->func = nullptr ;
            return true ;
        }
    }

    //operate command history
    if(lgr.back().is_containing(VKC_UP) && cmd_hist_index > 0) {
        cmd_hist_index -- ;
        lgr.remove_from_back(1) ;
        VirtualCmdLine::refresh() ;
        return true ;
    }
    if(lgr.back().is_containing(VKC_DOWN) && cmd_hist_index < cmd_hist.size() - 1) {
        cmd_hist_index ++ ;
        lgr.remove_from_back(1) ;
        VirtualCmdLine::refresh() ;
        return true ;
    }

    //invalid keys
    if(is_invalid_log(lgr, KeyBinder::InvalidPolicy::AllSystemKey) || lgr.size() > iParams::get_z("cmd_max_char")) {
        lgr.remove_from_back(1) ;
        return true ;
    }

    if(auto matched_func = KeyBinder::find_keybinds(lgr)) {
        if(matched_func->is_callable()) {
            p_cmdp->func = matched_func ;
            return true ;
        }
    }
    p_cmdp->func = nullptr ;
    return true ;
}


void CommandMode::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const parent_logger)
{
    using namespace CmdModeUtility ;
    if(!first_call) return ;
    
    using namespace ModeManager ;

    const auto past_mode = get_mode() ;
    change_mode(Mode::Command) ;

    VirtualCmdLine::reset() ;

    //refresh key state
    for(const auto& key : KeyAbsorber::get_pressed_list()) {
        KeybrdEventer::release_keystate(key) ;
    }

    MSG msg ;
    while(System::update_options()) {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;
            DispatchMessage(&msg) ;
        }

        try {VirtualCmdLine::cout(":" + cmd_hist.at(cmd_hist_index)->logger.get_as_str()) ;}
        catch(const std::out_of_range&) {VirtualCmdLine::clear() ;} ;

        if(!update_core_cmd()) break ;

        Sleep(10) ;
    }
    change_mode(past_mode) ;
}
