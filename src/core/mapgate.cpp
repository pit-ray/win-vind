#include "mapgate.hpp"

#include "bind/binded_func.hpp"
#include "err_logger.hpp"
#include "g_maps.hpp"
#include "key_absorber.hpp"
#include "key_log.hpp"
#include "key_logger_base.hpp"
#include "keycode_def.hpp"
#include "keycodecvt.hpp"
#include "logger_parser_mgr.hpp"
#include "mapdefs.hpp"
#include "mode.hpp"
#include "ntype_logger.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef DEBUG
#include "bindings_parser.hpp"
#endif


namespace
{
    using namespace vind ;
    using namespace vind::core ;

    class MapHook : public bind::BindedFunc {
    private:
        void do_process() const override {
        }
        void do_process(NTypeLogger&) const override {
        }
        void do_process(const CharLogger&) const override {
        }

    public:
        template <typename IDString>
        explicit MapHook(IDString&& id)
        : BindedFunc("maphook_" + id)
        {}

        virtual ~MapHook() noexcept = default ;

        MapHook(MapHook&&) = default ;
        MapHook& operator=(MapHook&&) = default ;

        MapHook(const MapHook&) = delete ;
        MapHook& operator=(const MapHook&) = delete ;
    } ;


    class MapHookReproduce : public bind::BindedFunc {
    private:
        Command cmd_ ;

        void do_process() const override {
            for(const auto& keyset : cmd_) {
                util::pushup(keyset.cbegin(), keyset.cend()) ;
            }
        }
        void do_process(NTypeLogger&) const override {
        }
        void do_process(const CharLogger&) const override {
        }

    public:
        template <typename IDString, typename TypeCommand>
        explicit MapHookReproduce(IDString&& id, TypeCommand&& cmd)
        : BindedFunc("maphook_reproduce_" + id),
          cmd_(std::forward<TypeCommand>(cmd))
        {}

        virtual ~MapHookReproduce() noexcept = default ;

        MapHookReproduce(MapHookReproduce&&) = default ;
        MapHookReproduce& operator=(MapHookReproduce&&) = default ;

        MapHookReproduce(const MapHookReproduce&) = delete ;
        MapHookReproduce& operator=(const MapHookReproduce&) = delete ;
    } ;


    NTypeLogger generate_stated_logger(const Command& cmd) {
        NTypeLogger lgr ;
        for(auto& keyset : cmd) {
            lgr.logging_state(KeyLog(keyset.cbegin(), keyset.cend())) ;
        }
        return lgr ;
    }

    using KeyUnorderedSet = std::unordered_set<KeyCode> ;
    using Key2KeysetMap = std::array<KeyUnorderedSet, 256> ;

    void solve_recursive_key2keyset_mapping(Key2KeysetMap& key2keyset_table) {
        for(KeyCode srckey = 1 ; srckey < 255 ; srckey ++) {
            auto dst = key2keyset_table[srckey] ;
            if(dst.empty()) {
                continue ;
            }

            while(true) {
                // Check mappings that refer to itself
                if(dst.find(srckey) != dst.end()) {
                    dst.clear() ;
                    break ;
                }

                KeyUnorderedSet mapped {} ;
                for(auto itr = dst.begin() ; itr != dst.end() ;) {
                    auto buf = key2keyset_table[*itr] ;
                    if(!buf.empty()) {
                        itr = dst.erase(itr) ;
                        mapped.merge(buf) ;
                    }
                    else {
                        itr ++ ;
                    }
                }

                if(mapped.empty()) {
                    break ;
                }

                dst.merge(mapped) ;
            }

            key2keyset_table[srckey] = std::move(dst) ;
        }
    }

    Command replace_command_with_key2keyset(
            const Command& src,
            const std::array<KeyUnorderedSet, 256>& key2keyset_table) {
        Command replaced_cmd{} ;
        for(const auto& keyset : src) {
            KeySet replaced_set{} ;
            for(const auto& key : keyset) {
                auto& mapset = key2keyset_table[key] ;
                if(!mapset.empty()) {
                    replaced_set.insert(
                            replaced_set.begin(),
                            mapset.begin(),
                            mapset.end()) ;
                }
                else {
                    replaced_set.push_back(key) ;
                }
            }

            util::remove_deplication(replaced_set) ;
            replaced_cmd.push_back(std::move(replaced_set)) ;
        }
        return replaced_cmd ;
    }

    std::unordered_map<std::size_t, Command>
    solve_recursive_cmd2cmd_mapping(
            const std::unordered_map<std::size_t, MapCell>& map_table,
            const std::array<KeyUnorderedSet, 256>& key2keyset_table) {

        std::unordered_map<std::size_t, NTypeLogger> loggers{} ;
        std::unordered_map<std::size_t, Command> target_cmds{} ;
        std::unordered_map<std::size_t, std::size_t> id_func2map{} ;

        LoggerParserManager solver{} ;
        std::vector<LoggerParser::SPtr> parsers{} ;

        // Setup LoggerParserManager for matching
        for(const auto& [mapid, map] : map_table) {
            auto func = std::make_shared<bind::BindedFunc>(
                    map.trigger_command_string()) ;
            auto funcid = func->id() ;

            id_func2map[funcid] = mapid ;
            target_cmds[funcid] = replace_command_with_key2keyset(
                                        map.target_command(), key2keyset_table) ;
            loggers[funcid]     = generate_stated_logger(target_cmds[funcid]) ;

            auto parser = std::make_unique<LoggerParser>(func) ;
            parser->append_binding(map.trigger_command()) ;
            parsers.push_back(std::move(parser)) ;
        }

        for(auto parser_itr = parsers.begin() ; parser_itr != parsers.end() ;) {
            auto func = (*parser_itr)->get_func() ;

            auto& lgr = loggers[func->id()] ;

            // Check if the logger will be mapped again after it has been mapped.
            while(true) {
                solver.reset_parser_states() ;

                bind::BindedFunc::SPtr matched = nullptr ;
                std::size_t matched_idx = 0 ;
                for(auto logitr = lgr.cbegin() ; logitr != lgr.cend() ; logitr ++) {
                    auto buf = solver.find_parser_with_transition(*logitr) ;
                    if(buf) {
                        matched = buf->get_func() ;
                        matched_idx = static_cast<std::size_t>(logitr - lgr.cbegin()) ;
                        break ;
                    }
                }

                if(!matched) {
                    parser_itr ++ ;
                    break ;
                }

                if(func->id() == matched->id()) {
                    // This func recursively remaps itself, so remove it from the output list.
                    parser_itr = parsers.erase(parser_itr) ;
                    break ;
                }

                auto& i_outcmd = target_cmds[func->id()] ;
                const auto& m_incmd = map_table.at(id_func2map[matched->id()]).trigger_command() ;
                auto& m_outcmd = target_cmds[matched->id()] ;

                Command merged ;
                for(std::size_t j = 0 ; j <= matched_idx ; j ++) {
                    KeySet mapped_set ;
                    for(auto key : i_outcmd[j]) {
                        for(auto matched_key : m_incmd[j]) {
                            if(matched_key != key) {
                                mapped_set.push_back(key) ;
                            }
                        }
                    }
                    merged.push_back(std::move(mapped_set)) ;
                }

                merged.insert(merged.begin(), m_outcmd.begin(), m_outcmd.end()) ;

                auto restnum = i_outcmd.size() - matched_idx - 1 ;
                if(restnum > 0) {
                    merged.insert(
                            merged.begin(),
                            i_outcmd.begin() + restnum,
                            i_outcmd.end()) ;
                }

                target_cmds[func->id()] = merged ;
                lgr = generate_stated_logger(merged) ;
            }
        }

        std::unordered_map<std::size_t, Command> solved_outcmd{} ;
        for(const auto& parser : parsers) {
            auto funcid = parser->get_func()->id() ;

            auto mapid = id_func2map[funcid] ;
            solved_outcmd[mapid] = std::move(target_cmds[funcid]) ;
        }
        return solved_outcmd ;
    }
}


namespace vind
{
    namespace core
    {
        struct MapGate::Impl {
            ModeArray<std::unordered_map<std::size_t, NTypeLogger>> lgrs_table_{} ;
            ModeArray<LoggerParserManager> mgr_table_{} ;

            ModeArray<std::array<KeyUnorderedSet, 256>> syncmap_table_{} ;
        } ;

        MapGate::MapGate()
        : pimpl(std::make_unique<Impl>())
        {}

        MapGate::~MapGate() noexcept = default ;

        MapGate& MapGate::get_instance() {
            static MapGate instance{} ;
            return instance ;
        }

        void MapGate::reconstruct(Mode mode) {
            std::unordered_map<std::size_t, MapCell> noremap_cmd2cmd{} ;
            std::unordered_map<std::size_t, MapCell> map_cmd2cmd{} ;
            std::array<MapCell, 256> map_key2keyset{} ;

            auto& loggers = pimpl->lgrs_table_[static_cast<int>(mode)] ;
            loggers.clear() ;

            auto& syncmap = pimpl->syncmap_table_[static_cast<int>(mode)] ;
            syncmap.fill(KeyUnorderedSet{}) ;

            std::vector<MapCell> maps ;
            get_maps(mode, maps) ;
            for(auto itr = std::make_move_iterator(maps.begin()) ;
                     itr != std::make_move_iterator(maps.end()) ; itr ++) {

                if(itr->is_noremap()) {
                    noremap_cmd2cmd[itr->in_hash()] = std::move(*itr) ;
                }
                else if(itr->is_map()) {
                    auto trigger_cmd = itr->trigger_command() ;
                    auto target_cmd = itr->target_command() ;

                    // The key2keyset and key2key are mapped synchronously.
                    if(trigger_cmd.size() == 1 && trigger_cmd.front().size() == 1  // trigger is key?
                            && target_cmd.size() == 1) {                           // target is keyset?
                        auto trigger_key = trigger_cmd.front().front() ;
                        auto target_keyset = target_cmd.front() ;

                        syncmap[trigger_key] = KeyUnorderedSet(
                                target_keyset.begin(), target_keyset.end()) ;

                        map_key2keyset[trigger_key] = std::move(*itr) ;
                    }
                    else {
                        map_cmd2cmd[itr->in_hash()] = std::move(*itr) ;
                    }
                }
            }

            solve_recursive_key2keyset_mapping(syncmap) ;
            for(int i = 1 ; i < 255 ; i ++) {
                if(!map_key2keyset[i].empty() && syncmap[i].empty()) {
                    PRINT_ERROR(
                            mode_to_prefix(mode) + "map " + \
                            map_key2keyset[i].trigger_command_string() + " " +
                            map_key2keyset[i].target_command_string() +
                            " recursively remaps itself.") ;
                }
            }

            auto solved_target_cmds = solve_recursive_cmd2cmd_mapping(map_cmd2cmd, syncmap) ;

            std::vector<LoggerParser::SPtr> parsers{} ;
            for(const auto& [mapid, map] : map_cmd2cmd) {
                if(solved_target_cmds.find(mapid) == solved_target_cmds.end()) {
                    PRINT_ERROR(
                            mode_to_prefix(mode) + "map " + \
                            map.trigger_command_string() + " " +
                            map.target_command_string() +
                            " recursively remaps itself.") ;
                    continue ;
                }

                auto func = std::make_shared<MapHookReproduce>(
                    map.trigger_command_string(), solved_target_cmds[mapid]) ;
                auto parser = std::make_shared<LoggerParser>(func) ;
                parser->append_binding(map.trigger_command()) ;
                parsers.push_back(std::move(parser)) ;
            }

            for(const auto& [mapid, map] : noremap_cmd2cmd) {
                auto func = std::make_shared<MapHook>(
                        map.trigger_command_string()) ;
                auto parser = std::make_shared<LoggerParser>(func) ;
                parser->append_binding(map.trigger_command()) ;
                parsers.push_back(std::move(parser)) ;
            }

            pimpl->mgr_table_[static_cast<int>(mode)] \
                = LoggerParserManager(std::move(parsers)) ;
        }

        void MapGate::reconstruct() {
            // Extraction of registered maps
            for(std::size_t mode = 0 ; mode < mode_num() ; mode ++) {
                reconstruct(static_cast<Mode>(mode)) ;
            }
        }

        const NTypeLogger& MapGate::map_logger(const NTypeLogger& lgr, Mode mode) {
            auto midx = static_cast<int>(mode) ;

            auto& mgr = pimpl->mgr_table_[midx] ;
            mgr.transition_parser_states_in_batch(lgr) ;

            auto maphook = mgr.find_accepted_parser() ;
            if(maphook) {
                auto func = maphook->get_func() ;
                func->process() ;

                auto& mapped_lgr = pimpl->lgrs_table_[midx][func->id()] ;
                if(lgr.has_head_num()) {
                    mapped_lgr.set_head_num(lgr.get_head_num()) ;
                }
                return mapped_lgr ;
            }
            return lgr ;
        }

        bool MapGate::map_syncstate(
                KeyCode hook_key,
                bool press_sync_state,
                Mode mode) {
            auto target = pimpl->syncmap_table_[static_cast<int>(mode)][hook_key] ;
            if(target.empty()) {
                return false ;
            }

            if(press_sync_state) {
                for(const auto& key : target) {
                    util::press_keystate(key, true) ;
                }
            }
            else {
                for(const auto& key : target) {
                    if(is_really_pressed(key) || is_pressed(key)) {
                        util::release_keystate(key, true) ;
                    }
                }
            }
            return true ;
        }
    }
}
