#include "key_binder.hpp"

#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include <windows.h>

#include "bindings_loader.hpp"
#include "change_mode.hpp"
#include "command.hpp"
#include "i_params.hpp"
#include "bindings_lists.hpp"
#include "key_binding.hpp"
#include "key_logger.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "path.hpp"
#include "virtual_cmd_line.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"

using namespace std ;

namespace KeyBinder
{
    template <typename T>
    using vplist_t = std::array<std::vector<T>, static_cast<std::size_t>(ModeManager::Mode::NUM)> ;
    static vplist_t<kbg::shp_t> vpbf{} ;

    static KeyLogger logger{} ;
    static kbg::shp_t running_func = nullptr ;

    void init() {
        using ModeManager::Mode ;

        auto setbf = [](const auto index, auto&& value) {
            return vpbf[static_cast<int>(index)] = value ;
        } ;

        setbf(Mode::Normal, BindingsLists::normal()) ;
        setbf(Mode::Insert, BindingsLists::insert()) ;
        setbf(Mode::Visual, BindingsLists::visual()) ;
        setbf(Mode::EdiNormal, BindingsLists::editor_normal()) ;
        setbf(Mode::EdiInsert, BindingsLists::editor_insert()) ;
        setbf(Mode::EdiVisual, BindingsLists::editor_visual()) ;
        setbf(Mode::EdiLineVisual, BindingsLists::editor_visual()) ;
    }

    void load_config() noexcept
    {
        BindingsLoader::load_config() ;

        decltype(auto) map = BindingsLoader::get_bindings() ;
        for(auto& v : vpbf) {
            for(auto& i : v) {
                try {i->set_command(map.at(i->name())) ;}
                catch(out_of_range&) {continue ;}
            }
        }
        CommandMode::load_config() ;
        JumpCursorUtility::load_config() ;
    }

    bool is_invalid_log(KeyLogger& lgr, const InvalidPolicy ip) noexcept {
        if(lgr.back().is_empty()) {
            return true ;
        }

        auto ignore = [&lgr](auto&& set) {
            return std::all_of(lgr.back().cbegin(), lgr.back().cend(), [&set](const auto& key) {
                return set.find(key) != set.end() ;
            }) ;
        } ;

        switch(ip) {
            case None: {
                return false ;
            }
            case AllSystemKey: {
                static const auto system_keys = VKCConverter::get_all_sys_vkc() ;
                return ignore(system_keys) ;
            }
            case UnbindedSystemKey: {
                return ignore(BindingsLoader::get_unbinded_syskeys()) ;
            }
            default: {
                return false ;
            }
        }
    }

    //This function regards as other functions is stronger than the running function.
    //If the 2nd argument is not passed, it regards as not processing.
    const kbg::shp_t find_keybinds(const KeyLogger& lgr) noexcept {
        std::size_t max_matching_num = 0 ;
        auto most_matched_func = running_func ;

        const auto& vp = vpbf[static_cast<int>(ModeManager::get_mode())] ;
        for(auto& func : vp) {
            if(running_func == func) {
                continue ;
            }

            const auto num = func->matched_num(lgr.back(), lgr.size() - 1) ;
            if(max_matching_num < num) {
                max_matching_num = num ;
                most_matched_func = func ;
            }
            else if(max_matching_num == num && func->is_callable()) {
                //on same matching level, a callable function ist the strongest.
                most_matched_func = func ;
            }
        }
        return most_matched_func ;
    }

    void call_matched_funcs() noexcept {
        static std::mutex mtx ;
        std::lock_guard<std::mutex> lock(mtx) ;

        if(!logger.is_changed_code()) {
            if(!running_func) {
                logger.remove_from_back(1) ;
                return ;
            }
            running_func->process(false) ;
            logger.remove_from_back(1) ;
            return ;
        }

        if(is_invalid_log(logger, InvalidPolicy::UnbindedSystemKey)) {
            logger.remove_from_back(1) ;
            running_func = nullptr ;
            return ;
        }

        auto matched_func = find_keybinds(logger) ;

        if(!matched_func) {
            logger.clear() ;
            return ;
        }

        if(matched_func->is_callable()) {
            matched_func->process(true) ;
            logger.clear() ;
            running_func = matched_func ;
        }
        else {
            running_func = nullptr ;
        }
    }
}
