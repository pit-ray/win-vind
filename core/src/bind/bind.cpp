#include "bind.hpp"

#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <windows.h>

#include "binded_func.hpp"

#include "i_params.hpp"
#include "path.hpp"
#include "err_logger.hpp"

#include "key/key_absorber.hpp"
#include "key/key_log.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "key/keycode_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#include "bindings_json_parser.hpp"
#include "bindings_lists.hpp"
#include "bindings_matcher.hpp"
#include "mode.hpp"

#include "mouse/jump_keybrd.hpp"
#include "proc/external_app.hpp"
#include "uia/easy_click.hpp"

// to use std::numeric_limits<T>::max()
#undef max

//internal linkage
namespace
{
    using namespace vind ;
    std::vector<BindedFunc::shp_t> g_func_list{} ;
    std::vector<BindedFunc::shp_t> g_all_func_list{} ;
    std::unordered_set<unsigned char> g_unbinded_syskeys{} ;
}

namespace vind
{
    namespace keybind {
        void initialize() {
            g_all_func_list = bindingslists::get() ;

            g_func_list.clear() ;

            g_unbinded_syskeys.clear() ;
            g_unbinded_syskeys = keycodecvt::get_all_sys_keycode() ;

            easyclick::initialize() ;
        }

        void load_config() {
            bindjsonparser::load_bindings_json(path::BINDINGS(), g_all_func_list, g_func_list) ;

            for(auto& func : g_func_list) {
                func->load_config() ;
            }
        }

        bool is_invalid_log(const KeyLog& log, const InvalidPolicy ip) {

            if(log.empty()) return true ;

            auto must_ignore = [&log](auto&& set) {
                return std::all_of(log.cbegin(), log.cend(), [&set](const auto& key) {
                    return set.find(key) != set.end() ;
                }) ;
            } ;

            switch(ip) {
                case InvalidPolicy::None: {
                    return false ;
                }
                case InvalidPolicy::AllSystemKey: {
                    static const auto system_keys = keycodecvt::get_all_sys_keycode() ;
                    return must_ignore(system_keys) ;
                }
                case InvalidPolicy::UnbindedSystemKey: {
                    return must_ignore(g_unbinded_syskeys) ;
                }
                default: {
                    return false ;
                }
            }
        }

        //This function regards as other functions is stronger than the running function.
        //If the 2nd argument is not passed, it regards as not processing.
        const BindedFunc::shp_t find_func(
                const KeyLoggerBase& lgr,
                const BindedFunc::shp_t& low_priority_func,
                const bool full_scan,
                mode::Mode mode) {

            unsigned int most_matched_num  = 0 ;
            BindedFunc::shp_t matched_func = nullptr ;
            BindedFunc::shp_t part_matched_func = nullptr ;

            auto choose = [&most_matched_num, &matched_func, &part_matched_func](auto& func, auto num) {
                if(num > most_matched_num) {
                    most_matched_num = num ;
                    matched_func     = func ;
                }
                else if(num == most_matched_num && func->is_callable()) {
                    //On same matching level, the callable function is the strongest.
                    matched_func = func ;
                }

                if(!matched_func && func->is_matched_syskey_in_combined_bindings()) {
                    part_matched_func = func ;
                }
            } ;

            if(!low_priority_func) { //lower cost version
                if(full_scan) {
                    for(const auto& func : g_func_list) {
                        choose(func, func->validate_if_fullmatch(lgr, mode)) ;
                    }
                }
                else {
                    for(const auto& func : g_func_list) {
                        choose(func, func->validate_if_match(lgr, mode)) ;
                    }
                }
                if(matched_func) {
                    return matched_func ;
                }
                return part_matched_func ;
            }

            unsigned int matched_num ;
            if(full_scan) {
                for(const auto& func : g_func_list) {
                    matched_num = func->validate_if_fullmatch(lgr, mode) ;
                    if(low_priority_func == func) continue ;
                    choose(func, matched_num) ;
                }
            }
            else {
                for(const auto& func : g_func_list) {
                    matched_num = func->validate_if_match(lgr, mode) ;
                    if(low_priority_func == func) continue ;
                    choose(func, matched_num) ;
                }
            }

            //New matched function is given priority over running func.
            if(matched_func) {
                return matched_func ;
            }

            if(low_priority_func->is_callable()) {
                return low_priority_func ;
            }

            if(part_matched_func) {
                return part_matched_func ;

            }

            return nullptr ;
        }

        const BindedFunc::shp_t find_func_byname(const std::string& name) {
                for(const auto& func : g_func_list) {
                    if(func->name() == name) return func ;
                }
                return nullptr ;
        }

        unsigned int extract_number_keycode(const KeyLog& log) {
            if(log.empty()) {
                return KEYCODE_UNDEFINED ;
            }

            for(const unsigned char& keycode : log) {
                if(keycodecvt::is_number(keycode)) {
                    return keycode ;
                }
            }

            return KEYCODE_UNDEFINED ;
        }

        void concatenate_keycode_as_number(unsigned int& number, unsigned char num_keycode) {
            if(!keycodecvt::is_number(num_keycode)) return ;
            constexpr auto max = std::numeric_limits<unsigned int>::max() / 10 ;
            if(number < max) { //prohibit to overflow
                number = number*10 + keycodecvt::to_number<unsigned int>(num_keycode) ;
            }
        }
    }
}


//internal linkage
namespace
{
    KeycodeLogger g_logger{} ;
    BindedFunc::shp_t g_running_func       = nullptr ;
    unsigned int g_repeat_num              = 0 ;
    bool g_must_release_key_after_repeated = false ;
}

namespace vind
{
    namespace keybind {
        void call_matched_funcs() {
            static const KeyLog lc_nums {
                KEYCODE_0, KEYCODE_1, KEYCODE_2, KEYCODE_3, KEYCODE_4,
                KEYCODE_5, KEYCODE_6, KEYCODE_7, KEYCODE_8, KEYCODE_9
            } ;

            g_logger.update() ;
            if(!g_logger.is_changed()) {
                if(!g_running_func) {
                    return ;
                }
                g_running_func->process(false, 1, &g_logger, nullptr) ;
                return ;
            }

            if(g_repeat_num != 0) {
                if(g_logger.latest().is_containing(KEYCODE_ESC)) {
                    g_repeat_num = 0 ;
                    VirtualCmdLine::reset() ;
                }
            }

            //Note
            //it ignores solo system keys.
            //Ex)
            //  ______________________________________________________
            // |                |                       |             |
            // |   input keys   |        Shift          |  Shift + t  |
            // |                | (unbinded key only)   |             | 
            // |----------------|-----------------------|-------------|
            // |   behavior     |        ignore         |    pass     |
            // |________________|_______________________|_____________|
            //
            if(is_invalid_log(g_logger.latest(), InvalidPolicy::UnbindedSystemKey)) {
                g_logger.remove_from_back(1) ;
                g_running_func = nullptr ;

                if(g_must_release_key_after_repeated) {
                    g_must_release_key_after_repeated = false ;
                }

                return ;
            }

            // Note about g_must_release_key_after_repeated:
            // false : same as default.
            // true  : wait until some unbinded sytem keys are inputed or no keys is inputed.
            // 
            // This behavior is needed to prohibit following case.
            // Ex)
            //  ________________________________________________________________________________________
            // |                            |      |         |                   |                      |
            // |         input keys         |  2   |  Shift  |      Shift + j    |         j            |
            // |----------------------------|------|---------|-------------------|----------------------|
            // | called func name (without) |  -   |    -    |  edi_n_remove_EOL | edi_move_caret_down  |
            // |----------------------------|------|---------|-------------------|----------------------|
            // | called func name (with)    |  -   |    -    |  edi_n_remove_EOL |          -           |
            // |____________________________|______|_________|___________________|______________________|
            //
            if(g_must_release_key_after_repeated) {
                g_logger.remove_from_back(1) ;
                g_running_func = nullptr ;
                return ;
            }
            auto topkeycode = *(g_logger.latest().begin()) ;

            //If some numbers has inputed, ignore commands binded by numbers.
            if(g_repeat_num != 0) {
                g_logger.latest() -= lc_nums ;
            }

            auto matched_func = find_func(g_logger, g_running_func) ;
            if(!matched_func) {
                if(!keycodecvt::is_number(topkeycode)) {
                    //If inputed non-numeric key, reset the repeat number.
                    if(g_repeat_num != 0) {
                        g_repeat_num = 0 ;
                        VirtualCmdLine::reset() ;
                    }
                }
                else {
                    constexpr auto max = std::numeric_limits<unsigned int>::max() / 10 ;
                    if(g_repeat_num < max && !mode::is_insert()) { //Whether it is not out of range?
                        g_repeat_num = g_repeat_num * 10 + keycodecvt::to_number<unsigned int>(topkeycode) ;
                        VirtualCmdLine::cout(std::to_string(g_repeat_num)) ;
                    }
                }

                g_logger.clear() ;
                g_running_func = nullptr ;
                return ;
            }

            if(matched_func->is_callable()) {
                g_running_func = matched_func ;

                if(g_repeat_num == 0) {
                    g_running_func->process(true, 1, &g_logger, nullptr) ;
                }
                else {
                    VirtualCmdLine::reset() ;
                    g_running_func->process(true, g_repeat_num, &g_logger, nullptr) ;
                    g_repeat_num = 0 ;
                    g_must_release_key_after_repeated = true ;
                }

                g_logger.clear() ;
                return ;
            }
        }
    }
}
