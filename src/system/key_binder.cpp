#include "key_binder.hpp"

#include <algorithm>
#include <iostream>
#include <mutex>
#include <deque>
#include <vector>
#include <array>
#include <memory>

#include <windows.h>

#include "bindings_loader.hpp"
#include "key_binding.hpp"
#include "command.hpp"
#include "mode_manager.hpp"
#include "key_logger.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"
#include "msg_logger.hpp"
#include "i_params.hpp"
#include "path.hpp"
#include "key_binder_list.hpp"

#include "virtual_cmd_line.hpp"
#include "change_mode.hpp"

using namespace std ;

struct KeyBinder::Impl
{
    template <typename T>
    using vplist_t = std::array<std::vector<T>, static_cast<std::size_t>(ModeManager::Mode::NUM)> ;
    vplist_t<kbg::shp_t> vpbf ;
    std::vector<cmd::shp_t> vpcmd ;

    KeyLogger logger ;
    kbg::shp_t running_func ;
    BindingsLoader parser ;

    struct CmdPoint {
        std::unique_ptr<KeyLogger> logger{std::make_unique<KeyLogger>()} ;
        cmd::shp_t func = nullptr ;
    } ;
    std::deque<std::shared_ptr<CmdPoint>> cmd_hist ;
    std::size_t cmd_hist_index ;

    void update_history() {
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

    explicit Impl()
    : vpbf(),
      vpcmd(),
      logger(),
      running_func(nullptr),
      parser(Path::BINDINGS()),
      cmd_hist{std::make_shared<CmdPoint>()},
      cmd_hist_index(0)
    {
        using ModeManager::Mode ;

        auto setbf = [this](const auto index, auto&& value) {
            return vpbf[static_cast<int>(index)] = value ;
        } ;

        setbf(Mode::Normal, KeyBinderList::normal()) ;
        setbf(Mode::Insert, KeyBinderList::insert()) ;
        setbf(Mode::Visual, KeyBinderList::visual()) ;
        setbf(Mode::EdiNormal, KeyBinderList::editor_normal()) ;
        setbf(Mode::EdiInsert, KeyBinderList::editor_insert()) ;
        setbf(Mode::EdiVisual, KeyBinderList::editor_visual()) ;
        setbf(Mode::EdiLineVisual, KeyBinderList::editor_visual()) ;

        vpcmd = KeyBinderList::command() ;
    }

    virtual ~Impl() noexcept
    {
        cmd_hist.clear() ;
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

void KeyBinder::load_config() noexcept
{
    if(!pimpl->parser.parse()) {
        return ;
    }
    decltype(auto) map = pimpl->parser.get_bindings() ;
    for(auto& v : pimpl->vpbf) {
        for(auto& i : v) {
            try {i->set_command(map.at(i->name())) ;}
            catch(out_of_range&) {continue ;}
        }
    }
    decltype(auto) cmd_map = pimpl->parser.get_commands() ;
    for(auto& i : pimpl->vpcmd) {
        try {i->set_command(cmd_map.at(i->name())) ;}
        catch(out_of_range&) {continue ;}
    }
    ExAppUtility::load_config() ;
    JumpCursorUtility::load_config() ;
}

namespace KBUtility{
    template <typename T>
    inline static auto is_ignored(const T& ignore_set, const KeyLog& log) noexcept {
        return std::all_of(log.cbegin(), log.cend(), [&ignore_set](const auto& key) {
            return ignore_set.find(key) != ignore_set.end() ;
        }) ;
    }
}

void KeyBinder::update_core() noexcept
{
    const auto& vp = pimpl->vpbf[static_cast<int>(ModeManager::get_mode())] ;
    using namespace KBUtility ;

    if(!pimpl->logger.is_changed_code()) {
        if(!pimpl->running_func) {
            pimpl->logger.remove_from_back(1) ;
            return ;
        }
        pimpl->running_func->process(false) ;
        pimpl->logger.remove_from_back(1) ;
        return ;
    }

    if(pimpl->logger.back().is_empty() ||
       is_ignored(pimpl->parser.get_ignored_syskeys(), pimpl->logger.back())) {
        //all is ignore code
        pimpl->logger.remove_from_back(1) ;
        pimpl->running_func = nullptr ;
        return ;
    }

    std::size_t max_matching_num = 0 ;
    auto most_matched_func = pimpl->running_func ;

    //overwrite callable
    for(auto& func : vp) {
        if(pimpl->running_func == func) {
            continue ;
        }

        const auto num = func->matched_num(pimpl->logger.back(), pimpl->logger.size() - 1) ;
        if(max_matching_num < num) {
            max_matching_num = num ;
            most_matched_func = func ;
        }
        else if(max_matching_num == num && func->is_callable()) {
            //on same matching level, a callable function ist the strongest.
            most_matched_func = func ;
        }
    }

    if(!most_matched_func) {
        pimpl->logger.clear() ;
        return ;
    }

    if(most_matched_func->is_callable()) {
        most_matched_func->process(true) ;
        pimpl->logger.clear() ;
        pimpl->running_func = most_matched_func ;
    }
    else {
        pimpl->running_func = nullptr ;
    }
}

void KeyBinder::update_core_cmd() noexcept
{
    auto return_mode = [] {
        const auto mode = ModeManager::get_mode() ;
        using ModeManager::Mode ;
        using ModeManager::change_mode ;
        if(mode == Mode::EdiCommand) {
            return Change2Editor::sprocess(true) ;
        }
        else {
            return Change2Normal::sprocess(true) ;
        }
    } ;

    static const auto ignore_alone = VKCConverter::get_all_sys_vkc() ;
    using namespace KBUtility ;

    auto& p_cmdp = pimpl->cmd_hist.at(pimpl->cmd_hist_index) ;
    auto& plger = p_cmdp->logger ;

    if(!plger->is_changed_char()) {
        plger->remove_from_back(1) ;
        return ;
    }
    if(plger->back().is_empty()) { //empty input is skipped
        plger->remove_from_back(1) ;
        return ;
    }

    if(plger->back().is_containing(VKC_ESC)){
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
        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;
        return ;
    }

    if(plger->back().is_containing(VKC_ENTER) && p_cmdp->func) {
        plger->remove_from_back(1) ; //remove keycode of enter

        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;
        return_mode() ;

        p_cmdp->func->process(plger->get_str()) ;
        pimpl->update_history() ;
        return ;
    }

    //edit command
    if(plger->back().is_containing(VKC_BKSPACE)) {
        plger->remove_from_back(2) ;
        VirtualCmdLine::refresh() ;

        if(plger->is_empty()) {
            p_cmdp->func = nullptr ;
            return ;
        }
    }

    //operate command history
    if(plger->back().is_containing(VKC_UP) && pimpl->cmd_hist_index > 0) {
        pimpl->cmd_hist_index -- ;
        plger->remove_from_back(1) ;
        VirtualCmdLine::refresh() ;
        return ;
    }
    if(plger->back().is_containing(VKC_DOWN) && pimpl->cmd_hist_index < pimpl->cmd_hist.size() - 1) {
        pimpl->cmd_hist_index ++ ;
        plger->remove_from_back(1) ;
        VirtualCmdLine::refresh() ;
        return  ;
    }

    //invalid keys
    if(is_ignored(ignore_alone, plger->back()) || plger->size() > iParams::get_z("cmd_max_char")) {
        plger->remove_from_back(1) ;
        return ;
    }

    //search cmd from cmd list
    for(auto& func : pimpl->vpcmd) {
        if(func->is_callable(*plger)) {
            p_cmdp->func = func ;
            return ;
        }
    }
    p_cmdp->func = nullptr ;
}

void KeyBinder::update() noexcept {
    if(ModeManager::is_command()) {
        try {VirtualCmdLine::cout(":" + pimpl->cmd_hist.at(pimpl->cmd_hist_index)->logger->get_str()) ;}
        catch(const std::out_of_range&) {VirtualCmdLine::clear() ;} ;

        update_core_cmd() ;
    }
    else {
        update_core() ;
    }
}
