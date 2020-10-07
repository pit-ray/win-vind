#include "key_binder.hpp"

#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

#include <windows.h>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "bindings_lists.hpp"
#include "change_mode.hpp"
#include "command.hpp"
#include "i_params.hpp"
#include "key_binding.hpp"
#include "key_logger.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "path.hpp"
#include "utility.hpp"
#include "virtual_cmd_line.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"

using namespace std ;

namespace KeyBinder
{
    static auto _vpbf{BindingsLists::get()} ;

    static KeyLogger _logger{} ;
    static BindedFunc::shp_t _running_func = nullptr ;

    static auto _unbinded_syskeys = VKCConverter::get_all_sys_vkc() ;

    using ModeManager::Mode ;
    static const std::unordered_map<Mode, const char*> _modeidxs {
        {Mode::Normal,          "guin"},
        {Mode::Insert,          "guii"},
        {Mode::Visual,          "guiv"},
        {Mode::Command,         "cmd"},
        {Mode::EdiNormal,       "edin"},
        {Mode::EdiInsert,       "edii"},
        {Mode::EdiVisual,       "ediv"},
        {Mode::EdiLineVisual,   "edivl"}
    } ;

    template <typename T>
    inline const auto _parse_strcmd(T&& cmdstr) {
        auto get_specode = [](std::string k) {
            if(k == "space")   return ' ' ;
            if(k == "hbar")    return '-' ;
            if(k == "gt")      return '>' ;
            if(k == "lt")      return '<' ;
            return static_cast<char>(0) ;
        } ;
        KeyMatcher::cmd_t cmd ;

        for(std::size_t i = 0 ; i < cmdstr.length() ; i ++) {
            const auto onechar = cmdstr[i] ;
            if(onechar != '<') {
                //ascii
                if(auto vkc = VKCConverter::get_vkc(onechar)) { //ex) a
                    cmd.emplace_back(1, vkc) ;
                    continue ;
                }

                //shifted ascii
                if(auto vkc = VKCConverter::get_shifted_vkc(onechar)) { //ex) A (A is divided to a and SHIFT)
                    cmd.push_back(KeyMatcher::keyset_t{vkc, VKC_SHIFT}) ;
                    continue ;
                }

                ERROR_PRINT(onechar + std::string(" of ") + cmdstr + "\tis invalid ascii key code") ;
                continue ;
            }

            auto pairpos = cmdstr.find('>', i + 1) ;
            if(pairpos == std::string::npos) {
                throw std::runtime_error("command is bad syntax. " + cmdstr +  " does not have a greater-than sign (>)") ;
            }

            KeyMatcher::keyset_t keyset ;
            const auto keystrset = Utility::split(cmdstr.substr(i + 1, pairpos - i - 1), "-") ;
            for(auto code = keystrset.begin() ; code != keystrset.end() ; code ++) {
                if(code != keystrset.begin() && code->length() == 1) { //ascii code
                    //ascii
                    if(auto vkc = VKCConverter::get_vkc(code->front())) {
                        keyset.push_back(vkc) ;
                        continue ;
                    }

                    //shifted ascii
                    if(auto vkc = VKCConverter::get_shifted_vkc(code->front())) {
                        keyset.push_back(vkc) ;
                        keyset.push_back(VKC_SHIFT) ;
                        continue ;
                    }
                }

                auto lowercode = Utility::A2a(*code) ;

                //if the cmd is same as some mode's key (e.g. <guin>, <edin>),
                //its pointer use same pointer to target mode.
                for(const auto& target_index : _modeidxs) {
                    if(lowercode == target_index.second) {
                        throw target_index.first ;
                    }
                }

                if(lowercode == "any") {
                    keyset.push_back(VKC_OPTIONAL) ;
                    continue ;
                }

                if(auto ascii = get_specode(lowercode)) {
                    if(auto vkc = VKCConverter::get_vkc(ascii)) {
                        keyset.push_back(vkc) ;
                        continue ;
                    }
                    if(auto vkc = VKCConverter::get_shifted_vkc(ascii)) {
                        keyset.push_back(vkc) ;
                        keyset.push_back(VKC_SHIFT) ;
                        continue ;
                    }

                    ERROR_PRINT(*code  + " is not supported. (" + Path::BINDINGS() + ")") ;
                    continue ;
                }

                if(const auto vkc = VKCConverter::get_sys_vkc(lowercode)) {
                    keyset.push_back(vkc) ;
                    _unbinded_syskeys.erase(vkc) ;
                    continue ;
                }

                ERROR_PRINT(*code + "\t of " + cmdstr + " is invalid system key code") ;
            }

            cmd.push_back(std::move(keyset)) ;

            i = pairpos ;
            continue ;
        }
        return cmd ;
    }

    void load_config() {
        std::ifstream ifs{Path::BINDINGS()} ;
        nlohmann::json jp ;
        ifs >> jp ;
        if(jp.empty()) {
            throw std::runtime_error(Path::BINDINGS() + " is empty.") ;
        }

        if(!jp.is_array()) {
            throw std::runtime_error("The root element of " + Path::BINDINGS() + " should be array.") ;
        }

        constexpr auto mode_num = static_cast<unsigned char>(Mode::NUM) ;

        std::array<KeyMatcher::shp_t, mode_num> matcher_list ;

        //if JSON's data is "edin": ["<guin>"], index_links[edin-index] = guin-index
        std::array<unsigned char, mode_num> index_links ;

        if(_vpbf.empty()) {
            throw std::logic_error("KeyBinder has no defined BindFunc.") ;
        }

        _unbinded_syskeys = VKCConverter::get_all_sys_vkc() ; //initialize

        //create name list
        std::unordered_map<std::string, BindedFunc::shp_t> funclist ;
        for(auto& func : _vpbf) {
            funclist[func->name()] = func ;
        }

        for(auto& obj : jp) {
            try {
                auto& func = funclist.at(obj.at("name")) ;
                if(!obj.is_object()) {
                    ERROR_PRINT("The child of root-array should be object. (" \
                            + Path::BINDINGS() + ", name: " + obj["name"].get<std::string>() + ").") ;
                }

                matcher_list.fill(nullptr) ;
                index_links.fill(static_cast<unsigned char>(Mode::None)) ;

                for(const auto& index : _modeidxs) {
                    try {
                        const auto& cmds = obj.at(index.second) ;
                        if(!cmds.is_array()) {
                            ERROR_PRINT("The command lists should be array (" \
                                    + func->name() + "/" + index.second + ").") ;
                            continue ;
                        }
                        if(cmds.empty()) {
                            continue ;
                        }
                        KeyMatcher::cmdlist_t cmdlist ;

                        for(std::string cmdstr : cmds) {
                            if(cmdstr.empty()) continue ;
                            KeyMatcher::cmd_t cmd ;
                            try {
                                cmd = _parse_strcmd(cmdstr) ;
                            }
                            catch(const std::runtime_error& e) {
                                ERROR_PRINT(func->name() + "::" + index.second\
                                        + " in " + Path::BINDINGS() + " " + e.what()) ;
                                continue ;
                            }
                            catch(const Mode m) {
                                index_links[static_cast<unsigned char>(index.first)] \
                                    = static_cast<unsigned char>(m) ;
                                cmdlist.clear() ;
                                break ;
                            }
                            cmdlist.push_back(cmd) ;
                        }

                        if(cmdlist.empty()) {
                            continue ;
                        }

                        matcher_list[static_cast<unsigned char>(index.first)] \
                            = std::make_shared<KeyMatcher>(std::move(cmdlist)) ;
                    }
                    catch(const std::out_of_range& e) {
                        ERROR_PRINT(e.what()) ;
                        continue ;
                    }
                }

                for(std::size_t i = 0 ; i < index_links.size() ; i ++) {
                    const auto link_idx = index_links[i] ;
                    if(link_idx == static_cast<unsigned char>(Mode::None)) continue ;
                    matcher_list[i] = matcher_list[link_idx] ;
                }

                for(std::size_t i = 0 ; i < matcher_list.size() ; i ++) {
                    func->register_matcher(i, matcher_list[i]) ;
                }
            }
            catch(const std::out_of_range& e) {
                ERROR_PRINT(std::string(e.what()) + ", so not loaded.") ;
                continue ;
            }
        }

        //post process
        JumpCursorUtility::load_config() ;
        ExAppUtility::load_config() ;
    }

    bool is_invalid_log(KeyLogger& lgr, const InvalidPolicy ip) noexcept {
        if(lgr.back().is_empty()) {
            return true ;
        }

        auto ignore = [&lgr](auto&& set) {
            return std::all_of(lgr.back().cbegin(), lgr.back().cend(), [&set](const auto& key) {
                return set.find(key) != set.end() || !VKCConverter::get_ascii(key) ;
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
                return ignore(_unbinded_syskeys) ;
            }
            default: {
                return false ;
            }
        }
    }

    //This function regards as other functions is stronger than the running function.
    //If the 2nd argument is not passed, it regards as not processing.
    const BindedFunc::shp_t find_keybinds(
            const KeyLogger& lgr,
            const BindedFunc::shp_t& current_func,
            ModeManager::Mode mode) {

        unsigned int most_matched_num = 0 ;
        auto matched_func = current_func ;

        for(const auto& func : _vpbf) {
            if(current_func == func) continue ;
            const auto matched_num = func->get_matched_num(lgr, mode) ;
            if(matched_num > most_matched_num) {
                most_matched_num = matched_num ;
                matched_func = func ;
            }
            else if(matched_num == most_matched_num && func->is_callable()) {
                //on same matching level, a callable function is the strongest.
                matched_func = func ;
            }
        }
        return matched_func ;
    }

    void call_matched_funcs() {
        static std::mutex mtx ;
        std::lock_guard<std::mutex> lock(mtx) ;

        if(!_logger.is_changed_code()) {
            if(!_running_func) {
                _logger.remove_from_back(1) ;
                return ;
            }
            _running_func->process(false, 1, &_logger) ;
            _logger.remove_from_back(1) ;
            return ;
        }

        if(is_invalid_log(_logger, InvalidPolicy::UnbindedSystemKey)) {
            _logger.remove_from_back(1) ;
            _running_func = nullptr ;
            return ;
        }

        auto matched_func = find_keybinds(_logger, _running_func) ;
        if(!matched_func) {
            _logger.clear() ;
            return ;
        }

        if(matched_func->is_callable()) {
            unsigned int repeat_num = 1 ; //in feature, set this variable

            _running_func = matched_func ;
            _running_func->process(true, repeat_num, &_logger) ;
            _logger.clear() ;
            return ;
        }
    }
}
