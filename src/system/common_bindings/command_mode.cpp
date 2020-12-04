#include "command_mode.hpp"

#include <windows.h>

#include <vector>

#include "binded_func.hpp"
#include "bindings_loader.hpp"
#include "i_params.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "key_binding.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "system.hpp"
#include "utility.hpp"
#include "virtual_cmd_line.hpp"
#include "vkc_converter.hpp"

namespace CmdMode
{
    struct CmdPoint {
        KeyLogger logger{} ;
        BindedFunc::shp_t func = nullptr ;
    } ;
    static std::deque<std::shared_ptr<CmdPoint>>
        cmd_hist{std::make_shared<CmdPoint>()} ;

    static std::size_t cmd_hist_index = 0 ;

    inline static void update_history() {
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

    static auto need_full_scan = false ;
}

const std::string CommandMode::sname() noexcept
{
    return "command_mode" ;
}

#define CONTINUE_LOOP (true)
#define BREAK_LOOP    (false)
inline static bool _main_loop() {
    using namespace CmdMode ;
    using Utility::remove_from_back ;

    auto& p_cmdp = cmd_hist.at(cmd_hist_index) ;
    auto& lgr    = p_cmdp->logger ;

    if(!KyLgr::log_as_char(lgr)) { //update log as character input
        remove_from_back(lgr, 1) ;
        return CONTINUE_LOOP ;
    }

    //canceling operation
    if(lgr.back().is_containing(VKC_ESC)){
        const auto recent_index = cmd_hist.size() - 1 ;
        if(cmd_hist_index == recent_index) {
            lgr.clear() ;
            p_cmdp->func = nullptr ;
        }
        else {
            remove_from_back(lgr, 1) ;
            cmd_hist_index = recent_index ;
        }
        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;
        return BREAK_LOOP ;
    }

    if(lgr.back().empty()) {
        remove_from_back(lgr, 1) ;
        return CONTINUE_LOOP ;
    }

    //decision of input
    if(lgr.back().is_containing(VKC_ENTER) && p_cmdp->func) {
        remove_from_back(lgr, 1) ; //remove keycode of enter

        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;

        p_cmdp->func->process(true, 1, nullptr, &lgr) ;
        update_history() ;
        return BREAK_LOOP ;
    }

    //edit command
    if(lgr.back().is_containing(VKC_BKSPACE)) {
        if(lgr.size() == 1) {
            lgr.clear() ;
            p_cmdp->func = nullptr ;
            VirtualCmdLine::clear() ;
            VirtualCmdLine::refresh() ;
            return BREAK_LOOP ;
        }

        remove_from_back(lgr, 2) ;
        VirtualCmdLine::refresh() ;
        need_full_scan = true ;
        return CONTINUE_LOOP ;
    }

    //command history operation
    if(lgr.back().is_containing(VKC_UP) && cmd_hist_index > 0) {
        cmd_hist_index -- ;
        remove_from_back(lgr, 1) ;
        VirtualCmdLine::refresh() ;
        return CONTINUE_LOOP ;
    }
    if(lgr.back().is_containing(VKC_DOWN) 
            && cmd_hist_index < cmd_hist.size() - 1) {
        cmd_hist_index ++ ;
        remove_from_back(lgr, 1) ;
        VirtualCmdLine::refresh() ;
        return CONTINUE_LOOP ;
    }

    //invalid keys
    if(is_invalid_log(lgr, KeyBinder::InvalidPolicy::AllSystemKey) ||
            lgr.size() > iParams::get_z("cmd_max_char")) {
        remove_from_back(lgr, 1) ;
        return CONTINUE_LOOP ;
    }

    auto matched_func = KeyBinder::find_func(lgr, p_cmdp->func, need_full_scan) ;
    need_full_scan = false ;

    if(matched_func) {
        if(matched_func->is_callable()) {
            p_cmdp->func = matched_func ;
            return CONTINUE_LOOP ;
        }
        p_cmdp->func = nullptr ;
    }
    return CONTINUE_LOOP ;
}


void CommandMode::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    using namespace CmdMode ;
    using namespace ModeManager ;

    if(!first_call) return ;

    const auto past_mode = get_mode() ;
    change_mode(Mode::Command) ;

    VirtualCmdLine::reset() ;
    KeyAbsorber::close_with_refresh() ;

    while(System::update_options() && _main_loop()) {
        Utility::get_win_message() ;

        VirtualCmdLine::cout(":" +
                KyLgr::log2str(cmd_hist.at(cmd_hist_index)->logger)) ;

        Sleep(10) ;
    }
    change_mode(past_mode) ;
}
