#include "funcfinder.hpp"

#include "bind/bindedfunc.hpp"
#include "bind/bindinglist.hpp"
#include "cmdparser.hpp"
#include "entry.hpp"
#include "keylgrbase.hpp"
#include "lgrparser.hpp"
#include "lgrparsermgr.hpp"
#include "maptable.hpp"
#include "mode.hpp"
#include "path.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#include <array>
#include <memory>
#include <stdexcept>
#include <unordered_map>


namespace vind
{
    namespace core
    {
        struct FuncFinder::Impl {
            std::vector<bind::BindedFunc::SPtr> funcs_{} ;
        } ;

        FuncFinder::FuncFinder()
        : pimpl(std::make_unique<Impl>())
        {}

        FuncFinder::~FuncFinder() noexcept = default ;
        FuncFinder::FuncFinder(FuncFinder&& rhs) = default ;
        FuncFinder& FuncFinder::operator=(FuncFinder&&) = default ;

        bind::BindedFunc::SPtr FuncFinder::find_func_byname(const std::string& name) const {
            auto id = bind::BindedFunc::name_to_id(name) ;
            for(const auto& func : pimpl->funcs_) {
                if(func->id() == id) {
                    return func ;
                }
            }

            return nullptr ;
        }

        void FuncFinder::reconstruct(Mode mode) {
            pimpl->funcs_ = bind::all_global_binded_funcs() ;
            do_reconstruct(mode) ;
        }

        void FuncFinder::reconstruct(
                Mode mode,
                const std::vector<bind::BindedFunc::SPtr>& funcs) {
            pimpl->funcs_ = funcs ;
            do_reconstruct(mode) ;
        }
        void FuncFinder::reconstruct(
                Mode mode,
                std::vector<bind::BindedFunc::SPtr>&& funcs) {
            pimpl->funcs_ = std::move(funcs) ;
            do_reconstruct(mode) ;
        }

        void FuncFinder::do_reconstruct(Mode mode) {
            auto& maptable = MapTable::get_instance() ;

            std::unordered_set<std::size_t> funcid_set ;
            for(const auto& func : pimpl->funcs_) {
                funcid_set.insert(func->id()) ;
            }

            std::unordered_map<std::string, \
                std::shared_ptr<CommandList>> funcmap ;

            auto maps = maptable.get_noremaps(mode) ;
            for(const auto& map : maps) {
                const auto func_name = util::A2a(map.target_command_string()) ;
                auto func_id = bind::BindedFunc::name_to_id(func_name) ;

                if(funcid_set.find(func_id) == funcid_set.end()) {
                    continue ;
                }

                auto& shared_cmd_list = funcmap[func_name] ;
                if(!shared_cmd_list) {
                    shared_cmd_list = std::make_shared<CommandList>() ;
                }
                shared_cmd_list->push_back(map.trigger_command()) ;
            }

            std::vector<LoggerParser::SPtr> parsers ;
            for(const auto& func : pimpl->funcs_) {
                try {
                    auto p_parser = std::make_shared<LoggerParser>(func) ;
                    p_parser->share_parsed_binding_list(funcmap.at(func->name())) ;
                    parsers.push_back(std::move(p_parser)) ;
                }
                catch(const std::out_of_range&) {
                    continue ;
                }
            }

            LoggerParserManager::operator=(
                    LoggerParserManager(std::move(parsers))) ;
        }
    }
}
