#include "mapgate.hpp"

#include "bind/binded_func.hpp"
#include "bind/safe_repeater.hpp"
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
        void do_process(core::NTypeLogger&) const override {
        }
        void do_process(const core::CharLogger&) const override {
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
        KeyLog empty_log{} ;
        for(auto& keyset : cmd) {
            //
            // To simulate the input, make a state transition with an empty log.
            // This is to make the logger recognize that it is a key release,
            // not a long pressing of the key.
            //
            lgr.logging_state(KeyLog(keyset.cbegin(), keyset.cend())) ;
            lgr.logging_state(empty_log) ;
        }
        return lgr ;
    }

    using Key2KeysetMap = std::array<KeySet, 256> ;

    /**
     *
     * It solves recursive mappings in key2keyset to a single mapping.
     * If a map is mapping to itself recursively, it will remove the map.
     *
     */
    void solve_recursive_key2keyset_mapping(Key2KeysetMap& key2keyset_table) {
        for(KeyCode srckey = 1 ; srckey < 255 ; srckey ++) {
            auto dst = key2keyset_table[srckey] ;
            if(dst.empty()) {
                continue ;
            }

            while(true) {
                // Check mappings that refer to itself
                if(std::find(dst.begin(), dst.end(), srckey) != dst.end()) {
                    dst.clear() ;
                    break ;
                }

                KeySet mapped {} ;
                for(auto itr = dst.begin() ; itr != dst.end() ;) {
                    auto buf = key2keyset_table[*itr] ;
                    if(!buf.empty()) {
                        itr = dst.erase(itr) ;
                        mapped.insert(mapped.begin(), buf.begin(), buf.end()) ;
                    }
                    else {
                        itr ++ ;
                    }
                }

                if(mapped.empty()) {
                    break ;
                }

                dst.insert(dst.begin(), mapped.begin(), mapped.end()) ;
            }

            util::remove_deplication(dst) ;
            key2keyset_table[srckey] = std::move(dst) ;
        }
    }

    /*
     *
     * Apply the key2keyset mapping to a command.
     *
     */
    Command replace_command_with_key2keyset(
            const Command& srccmd,
            const Key2KeysetMap& key2keyset_table) {
        Command replaced_cmd{} ;
        for(const auto& keyset : srccmd) {
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

    /**
     * It solves recursive mappings with the table of maps.
     *
     * @param[in] (map_table) MapCell unordered_map with MapCell::in_hash() as the key.
     * @param[in] (key2keyset_table) If you want to apply key2keyset mapping, specify this. Empty mappings will be ignored.
     * @return std::unordered_map<std::size_t, Command> Returns the solved map corresponding to the key of the map passed as the first argument.
     */
    std::unordered_map<std::size_t, Command>
    solve_recursive_cmd2cmd_mapping(
            const std::unordered_map<std::size_t, MapCell>& map_table,
            const Key2KeysetMap& key2keyset_table) {

        std::unordered_map<std::size_t, NTypeLogger> loggers{} ;
        std::unordered_map<std::size_t, Command> target_cmds{} ;
        std::unordered_map<std::size_t, std::size_t> id_func2map{} ;

        std::vector<LoggerParser::SPtr> parsers{} ;

        // Setup LoggerParserManager for matching
        for(const auto& [mapid, map] : map_table) {
            auto func = std::make_shared<bind::BindedFunc>(
                    "map{" + map.trigger_command_string() + "}") ;
            auto funcid = func->id() ;

            id_func2map[funcid] = mapid ;
            target_cmds[funcid] = replace_command_with_key2keyset(
                                        map.target_command(), key2keyset_table) ;
            loggers[funcid]     = generate_stated_logger(target_cmds[funcid]) ;

            auto parser = std::make_unique<LoggerParser>(func) ;
            parser->append_binding(map.trigger_command()) ;
            parsers.push_back(std::move(parser)) ;
        }

        LoggerParserManager solver{parsers} ;

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
                        if(buf->is_accepted()) {
                            matched = buf->get_func() ;
                            matched_idx = static_cast<std::size_t>(logitr - lgr.cbegin()) ;
                            break ;
                        }
                        else if(!buf->is_waiting()) {
                            solver.reset_parser_states() ;
                        }
                    }
                    else {
                        solver.reset_parser_states() ;
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
                const auto& m_outcmd = target_cmds.at(matched->id()) ;

                Command merged ;

                // Decide how much of the previous command you want to reuse.
                const auto matched_begin_idx = matched_idx - (m_incmd.size() - 1) ;
                for(std::size_t i = 0 ; i < matched_begin_idx ; i ++) {
                    merged.push_back(i_outcmd[i]) ;
                }
                for(auto i = matched_begin_idx ; i <= matched_idx ; i ++) {
                    KeySet mapped_set ;
                    const auto& m_keyset = *(m_incmd.cbegin() + i - matched_begin_idx) ;
                    for(auto key : i_outcmd[i]) {
                        if(std::find(m_keyset.cbegin(), m_keyset.cend(), key) == m_keyset.cend()) {
                            mapped_set.push_back(key) ;
                        }
                    }

                    if(!mapped_set.empty()) {
                        merged.push_back(std::move(mapped_set)) ;
                    }
                }

                // Append the mapping command.
                merged.insert(
                        merged.end(),
                        m_outcmd.begin(),
                        m_outcmd.end()) ;

                // Append the rest of the previous command.
                merged.insert(
                        merged.end(),
                        i_outcmd.begin() + matched_idx + 1,
                        i_outcmd.end()) ;

                i_outcmd = merged ;
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


    struct _MapGate {
        std::unordered_map<std::size_t, std::vector<KeyLog>> logpool_{} ;
        LoggerParserManager mgr_{} ;
        Key2KeysetMap syncmap_{} ;


        void reconstruct(Mode mode) {
            std::unordered_map<std::size_t, MapCell> noremap_cmd2cmd{} ;
            std::unordered_map<std::size_t, MapCell> map_cmd2cmd{} ;
            std::array<MapCell, 256> map_key2keyset{} ;

            syncmap_.fill(KeySet{}) ;

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

                        syncmap_[trigger_key] = KeySet(
                                target_keyset.begin(), target_keyset.end()) ;

                        map_key2keyset[trigger_key] = std::move(*itr) ;
                    }
                    else {
                        map_cmd2cmd[itr->in_hash()] = std::move(*itr) ;
                    }
                }
            }

            solve_recursive_key2keyset_mapping(syncmap_) ;
            for(int i = 1 ; i < 255 ; i ++) {
                if(syncmap_[i].empty()) {
                    if(!map_key2keyset[i].empty()) {
                        PRINT_ERROR(
                                mode_to_prefix(mode) + "map " + \
                                map_key2keyset[i].trigger_command_string() + " " +
                                map_key2keyset[i].target_command_string() +
                                " recursively remaps itself.") ;
                    }
                }
                else {
                    KeySet keys{} ;
                    KeySet syskeys{} ;

                    for(const auto& key : syncmap_[i]) {
                        if(get_ascii(key)) {
                            keys.push_back(key) ;
                        }
                        else {
                            syskeys.push_back(to_physical(key)) ;
                        }
                    }

                    syskeys.insert(syskeys.end(), keys.begin(), keys.end()) ;
                    syncmap_[i] = std::move(syskeys) ;
                }
            }

            logpool_.clear() ;

            auto solved_target_cmds = solve_recursive_cmd2cmd_mapping(map_cmd2cmd, syncmap_) ;

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

                for(const auto& keyset : solved_target_cmds[mapid]) {
                    logpool_[func->id()].emplace_back(keyset.begin(), keyset.end()) ;
                }
            }

            for(const auto& [mapid, map] : noremap_cmd2cmd) {
                auto func = std::make_shared<MapHook>(
                        map.trigger_command_string()) ;
                auto parser = std::make_shared<LoggerParser>(func) ;
                parser->append_binding(map.trigger_command()) ;
                parsers.push_back(std::move(parser)) ;

                for(const auto& keyset : map.target_command()) {
                    logpool_[func->id()].emplace_back(keyset.begin(), keyset.end()) ;
                }
            }

            mgr_ = LoggerParserManager(std::move(parsers)) ;
        }
    } ;
}


namespace vind
{
    namespace core
    {
        struct MapGate::Impl {
            ModeArray<_MapGate> c_{} ;
        } ;

        MapGate::MapGate()
        : pimpl(std::make_unique<Impl>())
        {}

        MapGate::~MapGate() noexcept = default ;

        MapGate& MapGate::get_instance() {
            static MapGate instance{} ;
            return instance ;
        }

        void MapGate::reconstruct() {
            // Extraction of registered maps
            for(std::size_t mode = 0 ; mode < mode_num() ; mode ++) {
                pimpl->c_[mode].reconstruct(static_cast<Mode>(mode)) ;
            }
        }

        std::vector<KeyLog> MapGate::map_logger(
                const KeyLog& log,
                Mode mode) {
            auto midx = static_cast<int>(mode) ;

            auto& mgr = pimpl->c_[midx].mgr_ ;

            auto parser = mgr.find_parser_with_transition(log) ;
            if(parser) {
                if(parser->is_accepted()) {
                    auto func = parser->get_func() ;
                    func->process() ;
                    mgr.reset_parser_states() ;
                    return pimpl->c_[midx].logpool_[func->id()] ;

                }
                else if(parser->is_rejected_with_ready()) {
                    mgr.reset_parser_states() ;
                }
            }
            else {
                mgr.reset_parser_states() ;
            }

            return std::vector<KeyLog>{} ;
        }

        bool MapGate::map_syncstate(
                KeyCode hook_key,
                bool press_sync_state,
                Mode mode) {
            auto midx = static_cast<int>(mode) ;
            auto target = pimpl->c_[midx].syncmap_[hook_key] ;
            if(target.empty()) {
                return false ;
            }

            if(press_sync_state) {
                open_some_ports(target.begin(), target.end()) ;
                util::press_keystate(target.begin(), target.end()) ;
                close_some_ports(target.begin(), target.end()) ;
            }
            else {
                open_some_ports(target.begin(), target.end()) ;
                util::release_keystate(target.begin(), target.end()) ;
                close_some_ports(target.begin(), target.end()) ;
            }
            return true ;
        }
    }
}
