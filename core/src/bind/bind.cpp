#include "bind/bind.hpp"

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
#include <map>
#include <vector>

#include <windows.h>

#include "bind/base/binded_func.hpp"

#include "coreio/i_params.hpp"
#include "coreio/path.hpp"
#include "coreio/err_logger.hpp"

#include "key/key_absorber.hpp"
#include "key/key_log.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "bind/base/keycode_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#include "bind/bindings_json_parser.hpp"
#include "bind/bindings_lists.hpp"
#include "bind/base/mode.hpp"

#include "bind/mouse/jump_keybrd.hpp"
#include "bind/proc/external_app.hpp"
#include "bind/uia/easy_click.hpp"

#include "bind/logger_parser.hpp"

// to use std::numeric_limits<T>::max()
#undef max

//internal linkage
namespace
{
    using namespace vind ;
    std::vector<BindedFunc::SPtr> g_all_func_list{} ;

    using LoggerParserList = std::vector<LoggerParser::SPtr> ;
    std::array<LoggerParserList, mode::mode_num()> g_mode_parser_list{} ;
}

namespace vind
{
    namespace keybind {
        void initialize() {
            g_all_func_list = bindingslists::get() ;

            for(auto& list : g_mode_parser_list) {
                list.clear() ;
            }

        }

        void load_config() {
            bindjsonparser::load_bindings_as_parser(path::BINDINGS(), g_all_func_list, g_mode_parser_list) ;
            for(auto& parser_list : g_mode_parser_list) {
                for(auto& parser : parser_list) {
                    parser->get_func()->load_config() ;
                }
            }
        }

        const BindedFunc::SPtr find_func_byname(const std::string& name) {
                for(const auto& func : g_all_func_list) {
                    if(func->name() == name) return func ;
                }
                return nullptr ;
        }

    }
}
