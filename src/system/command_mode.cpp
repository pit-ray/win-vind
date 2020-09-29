#include "command_mode.hpp"

#include <windows.h>

#include <vector>
#include <mutex>

#include "key_absorber.hpp"
#include "keybrd_eventer.hpp"
#include "system.hpp"
#include "virtual_cmd_line.hpp"
#include "i_params.hpp"
#include "mode_manager.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "bindings_loader.hpp"

#include "dedicate_to_window_ctrl.hpp"
#include "external_app.hpp"
#include "filer.hpp"
#include "mywindow_ctrl.hpp"
#include "switch_vdesktop.hpp"
#include "window_ctrl.hpp"

namespace CmdModeUtility
{
    static std::vector<cmd::shp_t> vpcmd = {
            SaveOpenedFile::create(),
            CloseOpenedFile::create(),
            CloseCurrentWindow::CommandWithCreator::create(),
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
            ExitConfigWindow::create(),
            OpenOtherFile::create(),
            MakeDir::create(),
            EnableTargetingOfDedicate2Window::CommandWithCreator::create(),
            DisableTargetingOfDedicate2Window::CommandWithCreator::create(),
            TaskView::CommandWithCreator::create()
    } ;

    struct CmdPoint {
        std::unique_ptr<KeyLogger> logger{std::make_unique<KeyLogger>()} ;
        cmd::shp_t func = nullptr ;
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

void CommandMode::load_config() noexcept {
    using namespace CmdModeUtility ;
    decltype(auto) map = BindingsLoader::get_commands() ;
    for(auto& i : vpcmd) {
        try {i->set_command(map.at(i->name())) ;}
        catch(const std::out_of_range&) {continue ;}
    }

    ExAppUtility::load_config() ;
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
    auto& plger = p_cmdp->logger ;

    if(!plger->is_changed_char()) {
        plger->remove_from_back(1) ;
        return true ;
    }

    if(plger->back().is_containing(VKC_ESC)){
        const auto recent_index = cmd_hist.size() - 1 ;
        if(cmd_hist_index == recent_index) {
            plger->clear() ;
            p_cmdp->func = nullptr ;
        }
        else {
            plger->remove_from_back(1) ;
            cmd_hist_index = recent_index ;
        }
        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;
        return false ;
    }

    if(plger->back().is_containing(VKC_ENTER) && p_cmdp->func) {
        plger->remove_from_back(1) ; //remove keycode of enter

        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;

        p_cmdp->func->process(plger->get_str()) ;
        update_history() ;
        return false ;
    }

    //edit command
    if(plger->back().is_containing(VKC_BKSPACE)) {
        plger->remove_from_back(2) ;
        VirtualCmdLine::refresh() ;

        if(plger->is_empty()) {
            p_cmdp->func = nullptr ;
            return true ;
        }
    }

    //operate command history
    if(plger->back().is_containing(VKC_UP) && cmd_hist_index > 0) {
        cmd_hist_index -- ;
        plger->remove_from_back(1) ;
        VirtualCmdLine::refresh() ;
        return true ;
    }
    if(plger->back().is_containing(VKC_DOWN) && cmd_hist_index < cmd_hist.size() - 1) {
        cmd_hist_index ++ ;
        plger->remove_from_back(1) ;
        VirtualCmdLine::refresh() ;
        return true ;
    }

    //invalid keys
    if(is_invalid_log(*plger, KeyBinder::InvalidPolicy::AllSystemKey) || plger->size() > iParams::get_z("cmd_max_char")) {
        plger->remove_from_back(1) ;
        return true ;
    }

    //search cmd from cmd list
    for(auto& func : vpcmd) {
        if(func->is_callable(*plger)) {
            p_cmdp->func = func ;
            return true ;
        }
    }
    p_cmdp->func = nullptr ;
    return true ;
}


bool CommandMode::sprocess(const bool first_call)
{
    using namespace CmdModeUtility ;
    if(!first_call) return true ;

    VirtualCmdLine::reset() ;

    //refresh key state
    for(const auto& key : KeyAbsorber::get_pressed_list()) {
        if(!KeybrdEventer::release_keystate(key)) {
            return false ;
        }
    }

    MSG msg ;
    while(System::update_options()) {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;
        }

        try {VirtualCmdLine::cout(":" + cmd_hist.at(cmd_hist_index)->logger->get_str()) ;}
        catch(const std::out_of_range&) {VirtualCmdLine::clear() ;} ;

        if(!update_core_cmd()) break ;

        Sleep(10) ;
    }
    return true ;
}
