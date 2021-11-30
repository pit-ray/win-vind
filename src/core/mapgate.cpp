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
#include "util/def.hpp"
#include "util/keybrd.hpp"

#include <unordered_map>
#include <unordered_set>


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

        template <typename IDString>
        static std::unique_ptr<bind::BindedFunc> create(IDString&& id) {
            return std::make_unique<MapHook>(std::forward<IDString>(id)) ;
        }

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

        template <typename CommandType>
        void set_output_command(CommandType&& cmd) {
            cmd_ = std::forward<CommandType>(cmd) ;
        }

        const Command& get_output_command() const noexcept {
            return cmd_ ;
        }

        virtual ~MapHookReproduce() noexcept = default ;

        template <typename IDString, typename TypeCommand>
        static std::unique_ptr<bind::BindedFunc> create(IDString&& id, TypeCommand&& cmd) {
            return std::make_unique<MapHookReproduce>(
                    std::forward<IDString>(id), std::forward<TypeCommand>(cmd)) ;
        }

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

    using SyncKeySet = std::unordered_set<KeyCode> ;
}


namespace vind
{
    namespace core
    {
        struct MapGate::Impl {
            ModeArray<std::unordered_map<std::size_t, NTypeLogger>> lgrs_table_{} ;
            ModeArray<LoggerParserManager> mgr_table_{} ;

            ModeArray<std::array<SyncKeySet, 256>> syncmap_table_{} ;
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
            ModeArray<std::vector<LoggerParser::SPtr>> parsers_table ;

            ModeArray<std::unordered_map<std::size_t, Command>> remap_triggers_table ;
            ModeArray<std::vector<LoggerParser::SPtr>> remap_parsers_table ;
            ModeArray<LoggerParserManager> remap_solver_table{} ;

            // Extraction of registered maps
            for(std::size_t i = 0 ; i < mode_num() ; i ++) {
                auto& loggers = pimpl->lgrs_table_[i] ;
                loggers.clear() ;

                auto& syncmap = pimpl->syncmap_table_[i] ;
                syncmap.fill(SyncKeySet{}) ;

                std::vector<UniqueMap> maps ;
                get_maps(static_cast<Mode>(i), maps) ;
                for(const auto& map : maps) {
                    if(map.is_noremap()) {
                        auto p_parser = std::make_shared<LoggerParser>(
                                MapHook::create(map.trigger_command_string())) ;

                        p_parser->append_binding(map.trigger_command()) ;
                        parsers_table[i].push_back(p_parser) ;

                        loggers[p_parser->get_func()->id()] \
                            = generate_stated_logger(map.target_command()) ;
                    }
                    else if(map.is_map()) {
                        auto trigger_cmd = map.trigger_command() ;
                        auto target_cmd = map.target_command() ;

                        // The key2keyset and key2key are mapped synchronously.
                        if(trigger_cmd.size() == 1 && trigger_cmd.front().size() == 1  // trigger is key?
                                && target_cmd.size() == 1) {                           // target is keyset?
                            auto trigger_key = trigger_cmd.front().front() ;
                            auto target_keyset = target_cmd.front() ;

                            syncmap[trigger_key] = SyncKeySet(
                                    target_keyset.begin(), target_keyset.end()) ;
                        }
                        else {
                            auto p_parser = std::make_shared<LoggerParser>(
                                    MapHookReproduce::create(map.target_command_string(), target_cmd)) ;

                            p_parser->append_binding(trigger_cmd) ;
                            remap_parsers_table[i].push_back(p_parser) ;

                            auto id = p_parser->get_func()->id() ;
                            remap_triggers_table[i][id] = trigger_cmd ;
                            loggers[id] = generate_stated_logger(target_cmd) ;
                        }
                    }
                }

                remap_solver_table[i] = LoggerParserManager(remap_parsers_table[i]) ;
            }

            // Solve recursive mapping in MapHookReproduce and syncmap.
            for(std::size_t i = 0 ; i < mode_num() ; i ++) {
                auto& solver = remap_solver_table[i] ;
                auto& remap_parsers = remap_parsers_table[i] ;
                auto& remap_triggers = remap_triggers_table[i] ;

                // Solve recursive MapHookReproduce
                for(auto parser_itr = remap_parsers.begin() ; parser_itr != remap_parsers.end() ;) {
                    auto func = std::dynamic_pointer_cast<MapHookReproduce>((*parser_itr)->get_func()) ;
                    auto& lgr = pimpl->lgrs_table_[i][func->id()] ;

                    // Check if the logger will be mapped again after it has been mapped.
                    while(true) {
                        solver.reset_parser_states() ;

                        std::shared_ptr<MapHookReproduce> matched = nullptr ;
                        std::size_t matched_idx = 0 ;
                        for(auto logitr = lgr.cbegin() ; logitr != lgr.cend() ; logitr ++) {
                            auto buf = solver.find_parser_with_transition(*logitr) ;
                            if(buf) {
                                matched = std::dynamic_pointer_cast<MapHookReproduce>(buf->get_func()) ;
                                matched_idx = static_cast<std::size_t>(logitr - lgr.cbegin()) ;
                                break ;
                            }
                        }

                        if(!matched) {
                            parser_itr ++ ;
                            break ;
                        }

                        if(func->id() == matched->id()) {
                            PRINT_ERROR(
                                    "{" + func->name() + "} "
                                    "recursively remaps itself in " + core::mode_to_name(i) + ".") ;
                            parser_itr = remap_parsers.erase(parser_itr) ;
                            break ;
                        }

                        decltype(auto) outcmd = func->get_output_command() ;
                        decltype(auto) matched_incmd = remap_triggers[func->id()] ;
                        decltype(auto) matched_outcmd = matched->get_output_command() ;

                        Command merged ;

                        for(std::size_t j = 0 ; j <= matched_idx ; j ++) {
                            KeySet mapped_set ;
                            for(auto key : outcmd[j]) {

                                for(auto matched_key : matched_incmd[j]) {
                                    if(matched_key != key) mapped_set.push_back(key) ;
                                }
                            }
                            merged.push_back(std::move(mapped_set)) ;
                        }

                        merged.insert(
                                merged.begin(),
                                matched_outcmd.begin(),
                                matched_outcmd.end()) ;

                        auto restnum = outcmd.size() - matched_idx - 1 ;
                        if(restnum > 0) {
                            merged.insert(
                                    merged.begin(),
                                    outcmd.begin() + restnum,
                                    outcmd.end()) ;
                        }

                        func->set_output_command(merged) ;
                        lgr = generate_stated_logger(merged) ;
                    }
                }

                // Register solved parsers and generate each manager.
                auto& parsers = parsers_table[i] ;
                parsers.insert(
                        parsers.begin(),
                        remap_parsers.begin(),
                        remap_parsers.end()) ;
                pimpl->mgr_table_[i] = LoggerParserManager(parsers) ;


                // Solve recursive synchronous mapping.
                auto& syncmap = pimpl->syncmap_table_[i] ;
                for(KeyCode j = 1 ; j < 255 ; j ++) {
                    auto dst = syncmap[j] ;
                    if(dst.empty()) {
                        continue ;
                    }

                    while(true) {
                        // Check mappings that refer to itself
                        if(dst.find(j) != dst.end()) {
                            PRINT_ERROR(
                                    "{" + get_keycode_name(j) + "} "
                                    "recursively remaps itself in " + core::mode_to_name(i) + ".") ;
                            dst.clear() ;
                            break ;
                        }

                        SyncKeySet mapped {} ;
                        for(auto itr = dst.begin() ; itr != dst.end() ;) {
                            auto buf = syncmap[*itr] ;
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

                    syncmap[j] = std::move(dst) ;
                }
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
