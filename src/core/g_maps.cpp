#include "g_maps.hpp"

#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "util/disable_gcc_warning.hpp"

#include <nlohmann/json.hpp>

#include "util/enable_gcc_warning.hpp"

#include "bind/bindings_lists.hpp"
#include "bindings_parser.hpp"
#include "err_logger.hpp"
#include "mapdefs.hpp"
#include "mode.hpp"
#include "path.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace
{
    using namespace vind ;

    template <typename T>
    bool is_func_name(T&& name) {
        auto id = bind::BindedFunc::name_to_id(std::forward<T>(name)) ;
        for(const auto& f : bind::all_global_binded_funcs()) {
            if(id == f->id()) return true ;
        }
        return false ;
    }

    using MapModeList = ModeArray<std::unordered_map<std::size_t, core::MapCell>> ;
    MapModeList g_mode_maps{} ;

    using namespace nlohmann ;
    json g_default_maps{} ;
}


namespace vind
{
    namespace core
    {
        struct MapCell::Impl {
            std::string instr_ ;
            Command in_ ;
            std::size_t in_hash_ ;

            std::string outstr_ ;
            Command out_ ;
            std::size_t out_hash_ ;

            MapType type_ ;

            template <typename T1, typename T2>
            explicit Impl(
                    T1&& in,
                    T2&& out,
                    MapType expect_type)
            : instr_(std::forward<T1>(in)),
              in_(parse_string_binding(instr_)),
              in_hash_(0),
              outstr_(std::forward<T2>(out)),
              out_(),
              out_hash_(0),
              type_(expect_type)
            {}

            explicit Impl()
            : instr_(),
              in_(),
              in_hash_(0),
              outstr_(),
              out_(),
              out_hash_(0),
              type_(MapType::UNDEFINED)
            {}
        } ;

        MapCell::MapCell()
        : pimpl(std::make_shared<Impl>())
        {}

        MapCell::MapCell(
                const std::string& in,
                const std::string& out,
                MapType expect_type,
                bool check_if_func)
        : pimpl(std::make_shared<Impl>(in, out, expect_type))
        {
            for(auto& keyset : pimpl->in_) {
                if(keyset.empty()) {
                    throw LOGIC_EXCEPT("Invalid Trigger Command") ;
                }
            }

            if(pimpl->type_ == MapType::NOREMAP && check_if_func) {
                if(is_func_name(out)) {
                    pimpl->type_ = MapType::NOREMAP_FUNCTION ;
                }
            }

            if(pimpl->type_ == MapType::MAP || pimpl->type_ == MapType::NOREMAP) {
                pimpl->out_ = parse_string_binding(pimpl->outstr_) ;
                if(pimpl->in_ == pimpl->out_) {
                    throw LOGIC_EXCEPT("Mapping between the same command is not allowed.") ;
                }
            }
        }

        bool MapCell::is_noremap_function() const noexcept {
            return pimpl->type_ == MapType::NOREMAP_FUNCTION ;
        }

        bool MapCell::is_noremap() const noexcept {
            return pimpl->type_ == MapType::NOREMAP ;
        }

        bool MapCell::is_map() const noexcept {
            return pimpl->type_ == MapType::MAP ;
        }

        const Command& MapCell::trigger_command() const noexcept {
            return pimpl->in_ ;
        }

        const std::string& MapCell::trigger_command_string() const noexcept {
            return pimpl->instr_ ;
        }

        const Command& MapCell::target_command() const {
            return pimpl->out_ ;
        }

        const std::string& MapCell::target_command_string() const noexcept {
            return pimpl->outstr_ ;
        }

        bool MapCell::empty() const noexcept {
            return pimpl->in_.empty() || pimpl->outstr_.empty() ;
        }

        std::size_t MapCell::out_hash() const {
            if(pimpl->out_hash_ == 0) {
                if(is_noremap_function()) {
                    pimpl->out_hash_ = compute_hash(pimpl->outstr_) ;
                }
                else {
                    pimpl->out_hash_ = compute_hash(pimpl->out_) ;
                }
            }
            return pimpl->out_hash_ ;
        }

        std::size_t MapCell::in_hash() const {
            if(pimpl->in_hash_ == 0) {
                pimpl->in_hash_ = compute_hash(pimpl->in_) ;
            }
            return pimpl->in_hash_ ;
        }

        std::size_t MapCell::compute_hash(const std::string& strcmd) {
            return compute_hash(parse_string_binding(strcmd)) ;
        }

        std::size_t MapCell::compute_hash(const Command& cmd) {
            std::string strcmd{} ;
            for(auto& set : cmd) {
                for(auto& key : set) {
                    strcmd.push_back(key) ;
                }
            }
            return std::hash<std::string>()(std::move(strcmd)) ;
        }

        bool MapCell::operator==(MapCell&& rhs) const {
            return in_hash() == rhs.in_hash() && out_hash() == rhs.out_hash() ;
        }

        bool MapCell::operator==(const MapCell& rhs) const {
            return in_hash() == rhs.in_hash() && out_hash() == rhs.out_hash() ;
        }

        bool MapCell::operator!=(MapCell&& rhs) const {
            return in_hash() != rhs.in_hash() || out_hash() != rhs.out_hash() ;
        }

        bool MapCell::operator!=(const MapCell& rhs) const {
            return in_hash() != rhs.in_hash() || out_hash() != rhs.out_hash() ;
        }


        void initialize_maps() {
            g_default_maps.clear() ;
            std::ifstream ifs(BINDINGS_DEFAULT()) ;
            ifs >> g_default_maps ;
        }

        void reset_all_maps() {
            MapModeList().swap(g_mode_maps) ;

            for(const auto& obj : g_default_maps) {
                std::size_t modeidx ;

                auto pretty_error = [&modeidx, &obj] (auto& e) {
                    std::stringstream ss ;
                    ss << e.what() << " (" << mode_to_prefix(modeidx) \
                        << MAP_DEFINE_KEYWORD_IN_JSON \
                        << " in" << obj.dump() << ")" ;
                    return ss.str() ;
                } ;

                try {
                    if(!obj.is_object()) {
                        PRINT_ERROR("Invalid Syntax " + obj.dump()) ;
                        continue ;
                    }

                    auto name = util::A2a(obj.at("name").get<std::string>()) ;
                    if(name.empty()) {
                        throw LOGIC_EXCEPT("Invalid Function Name") ;
                    }

                    for(modeidx = 0 ; modeidx < mode_num() ; modeidx ++) {
                        auto modekey = mode_to_prefix(modeidx) + MAP_DEFINE_KEYWORD_IN_JSON ;
                        if(modekey.empty()) {
                            continue ;
                        }

                        if(!obj.contains(modekey)) {
                            continue ;
                        }

                        auto cmdlist = obj[modekey] ;
                        if(cmdlist.empty() || !cmdlist.is_array()) {
                            continue ;
                        }

                        //If there are some key-bindings fields of the mode having <mode-name> (e.q. <guin>, <edin>) in bindings.json ,
                        //they are copied key-bindings from the first mode in json-array to them.
                        //Ex) "guin": ["<Esc>", "happy"]
                        //    "edin": ["<guin>", "<guii>"]    -> same as "guin"'s key-bindings(<Esc>, "happy")
                        if(cmdlist.size() == 1) {
                            auto copy_mode = parse_string_modecode(
                                    cmdlist.front(), MAP_DEFINE_KEYWORD_IN_JSON) ;
                            if(copy_mode != Mode::UNDEFINED) {
                                cmdlist = obj.at(mode_to_prefix(copy_mode) + MAP_DEFINE_KEYWORD_IN_JSON) ;
                            }
                        }

                        if(!is_func_name(name)) {
                            throw std::out_of_range("Unknown name: " + name) ;
                        }

                        auto& maps = g_mode_maps[modeidx] ;
                        for(std::string strcmd : cmdlist) {
                            if(strcmd.empty()) {
                                continue ;
                            }
                            MapCell map(strcmd, name, MapType::NOREMAP_FUNCTION, false) ;
                            maps[map.in_hash()] = std::move(map) ;
                        }
                    }
                }
                catch(const json::out_of_range& e) {
                    PRINT_ERROR(pretty_error(e)) ;
                    continue ;
                }
                catch(const std::out_of_range& e) {
                    PRINT_ERROR(pretty_error(e)) ;
                    continue ;
                }
                catch(const std::logic_error& e) {
                    throw std::logic_error(pretty_error(e)) ;
                }
                catch(const std::runtime_error& e) {
                    throw std::runtime_error(pretty_error(e)) ;
                }
            }
        }

        void do_map(
                const std::string& incmd,
                const std::string& outcmd,
                Mode mode) {
            if(incmd.empty() || outcmd.empty()) {
                throw RUNTIME_EXCEPT("Empty maps") ;
            }

            MapCell map(incmd, outcmd, MapType::MAP, false) ;
            // Overwrite map anyway
            g_mode_maps[static_cast<int>(mode)][map.in_hash()] = std::move(map) ;
        }

        void do_noremap(
                const std::string& incmd,
                const std::string& outcmd,
                Mode mode) {
            if(incmd.empty() || outcmd.empty()) {
                throw RUNTIME_EXCEPT("Empty maps") ;
            }

            MapCell map(incmd, outcmd, MapType::NOREMAP, true) ;

            if(map.is_noremap_function()) {
                // Overwrite if function-name or empty
                auto& maps = g_mode_maps[static_cast<int>(mode)] ;
                try {
                    auto& oldmap = maps.at(map.in_hash()) ;
                    if(oldmap.is_noremap_function()) {
                        oldmap = std::move(map) ;
                    }
                }
                catch(const std::out_of_range&) {
                    maps[map.in_hash()] = std::move(map) ;
                }
            }
            else {
                // Overwrite map anyway
                g_mode_maps[static_cast<int>(mode)][map.in_hash()] = std::move(map) ;
            }
        }

        void do_unmap(
                const std::string& incmd,
                Mode mode) {
            if(incmd.empty()) {
                throw RUNTIME_EXCEPT("Empty map") ;
            }
            auto hash = MapCell::compute_hash(incmd) ;

            g_mode_maps[static_cast<int>(mode)].erase(hash) ;
        }

        void do_mapclear(Mode mode) {
            g_mode_maps[static_cast<int>(mode)].clear() ;
        }

        MapCell get_map(
                const std::string& cmd,
                Mode mode) {
            auto hash = MapCell::compute_hash(cmd) ;
            return g_mode_maps[static_cast<int>(mode)].at(hash) ;
        }

        void get_maps(
                Mode mode,
                std::vector<MapCell>& returns) {
            if(!returns.empty()) {
                returns.clear() ;
            }
            for(auto& [hash, map] : g_mode_maps[static_cast<int>(mode)]) {
                returns.push_back(map) ;
            }
        }
    }
}
