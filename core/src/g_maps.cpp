#include "g_maps.hpp"

#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "disable_gcc_warning.hpp"

#include <nlohmann/json.hpp>

#include "enable_gcc_warning.hpp"

#include "bind/bindings_lists.hpp"
#include "err_logger.hpp"
#include "mapdefs.hpp"
#include "mode.hpp"
#include "parser/bindings_parser.hpp"
#include "path.hpp"
#include "util/def.hpp"


namespace
{
    using namespace vind ;

    const auto& func_list = bindingslists::get() ;
    bool is_func_name(const std::string& name) {
        auto id = std::hash<std::string>()(name) ;
        for(const auto& f : func_list) {
            if(id == f->id()) return true ;
        }
        return false ;
    }


    using MapModeList = ModeArray<std::unordered_map<std::size_t, gmaps::UniqueMap>> ;
    MapModeList g_mode_maps{} ;

    using namespace nlohmann ;
    json g_default_maps{} ;
}


namespace vind
{
    namespace gmaps {
        struct UniqueMap::Impl {
            Command in_ ;
            std::string out_ ;
            bool is_function_ ;

            explicit Impl()
            : in_(),
              out_(),
              is_function_(false)
            {}

            template <typename T1, typename T2>
            explicit Impl(T1&& in, T2& out)
            : in_(bindparser::parse_string_binding(std::forward<T1>(in))),
              out_(std::forward<T2>(out)),
              is_function_(is_func_name(out_))
            {}
            template <typename T1, typename T2>
            explicit Impl(T1&& in, T2& out, bool is_function)
            : in_(bindparser::parse_string_binding(std::forward<T1>(in))),
              out_(std::forward<T2>(out)),
              is_function_(is_function)
            {}

        } ;

        UniqueMap::UniqueMap()
        : pimpl(std::make_shared<Impl>())
        {}

        UniqueMap::UniqueMap(
                const std::string& in,
                const std::string& out)
        : pimpl(std::make_shared<Impl>(in, out))
        {}

        UniqueMap::UniqueMap(
                const std::string& in,
                const std::string& out,
                bool is_function)
        : pimpl(std::make_shared<Impl>(in, out, is_function))
        {}

        bool UniqueMap::is_function() const noexcept {
            return pimpl->is_function_ ;
        }

        const Command& UniqueMap::trigger_command() const noexcept {
            return pimpl->in_ ;
        }

        const std::string& UniqueMap::func_name() const noexcept {
            return pimpl->out_ ;
        }

        std::size_t UniqueMap::func_id() const noexcept {
            return BindedFunc::name_to_id(pimpl->out_) ;
        }

        Command UniqueMap::create_target_command() const {
            return bindparser::parse_string_binding(pimpl->out_) ;
        }

        std::size_t UniqueMap::compute_hash() const {
            std::string strcmd{} ;
            for(auto& set : pimpl->in_) {
                for(auto& key : set) {
                    strcmd.push_back(key) ;
                }
            }
            return std::hash<std::string>()(std::move(strcmd)) ;
        }


        void initialize() {
            g_default_maps.clear() ;
            std::ifstream ifs(path::to_u8path(path::Default::BINDINGS())) ;
            ifs >> g_default_maps ;
        }

        void reset() {
            MapModeList().swap(g_mode_maps) ;

            for(const auto& obj : g_default_maps) {
                try {
                    if(!obj.is_object()) {
                        PRINT_ERROR("Invalid Syntax " + obj.dump()) ;
                        continue ;
                    }

                    const auto& name = obj.at("name") ;
                    for(std::size_t i = 0 ; i < mode::mode_num() ; i ++) {

                        auto modekey = mode::get_mode_strcode(i) ;
                        if(modekey.empty()) {
                            continue ;
                        }

                        const auto& cmdlist = obj.at(modekey) ;
                        if(cmdlist.empty() || !cmdlist.is_array()) {
                            continue ;
                        }

                        auto& maps = g_mode_maps[i] ;
                        for(auto& strcmd : cmdlist) {
                            UniqueMap map(strcmd, name, true) ;
                            maps[map.compute_hash()] = std::move(map) ;
                        }
                    }
                }
                catch(const std::out_of_range& e) {
                    PRINT_ERROR(e.what()) ;
                    continue ;
                }
                catch(const json::out_of_range& e) {
                    PRINT_ERROR(e.what()) ;
                    continue ;
                }
            }
        }

        void map(
                const std::string& incmd,
                const std::string& outcmd,
                mode::Mode mode) {
            if(incmd.empty() || outcmd.empty()) {
                throw RUNTIME_EXCEPT("Empty maps") ;
            }

            UniqueMap map(incmd, outcmd) ;

            if(map.is_function()) {
                // Overwrite if function-name or empty
                auto hash = map.compute_hash() ;

                auto& maps = g_mode_maps[static_cast<int>(mode)] ;
                try {
                    auto& oldmap = maps.at(hash) ;
                    if(oldmap.is_function()) {
                        oldmap = std::move(map) ;
                    }
                }
                catch(const std::out_of_range&) {
                    maps[hash] = std::move(map) ;
                }
            }
            else {
                // Overwrite map anyway
                g_mode_maps[static_cast<int>(mode)][map.compute_hash()] = std::move(map) ;
            }
        }

        void unmap(
                const std::string& incmd,
                mode::Mode mode) {
            if(incmd.empty()) {
                throw RUNTIME_EXCEPT("Empty map") ;
            }
            UniqueMap map(incmd, "", true) ;
            auto hash = map.compute_hash() ;

            g_mode_maps[static_cast<int>(mode)].erase(hash) ;
        }

        void mapclear(mode::Mode mode) {
            g_mode_maps[static_cast<int>(mode)].clear() ;
        }

        UniqueMap get_map(
                const std::string& cmd,
                mode::Mode mode) {
            UniqueMap map(cmd, "", true) ;
            return g_mode_maps[static_cast<int>(mode)].at(map.compute_hash()) ;
        }

        void get_maps(
                mode::Mode mode,
                std::vector<UniqueMap>& returns) {
            returns.clear() ;
            for(auto& [hash, map] : g_mode_maps[static_cast<int>(mode)]) {
                returns.push_back(map) ;
            }
        }
    }
}
