#include "inputgate.hpp"

#include "bind/bindedfunc.hpp"
#include "bind/bindinglist.hpp"
#include "bind/saferepeat.hpp"
#include "errlogger.hpp"
#include "keycodedef.hpp"
#include "keylgrbase.hpp"
#include "keylog.hpp"
#include "lgrparsermgr.hpp"
#include "maptable.hpp"
#include "mode.hpp"
#include "ntypelogger.hpp"
#include "util/container.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/interval_timer.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/winwrap.hpp"

#include <windows.h>

#include <array>
#include <chrono>
#include <functional>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>


#define KEYUP_MASK 0x0001


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

        SystemCall do_process() const override {
            for(const auto& keyset : cmd_) {
                InputGate::get_instance().pushup(keyset.cbegin(), keyset.cend()) ;
            }
            return SystemCall::NOTHING ;
        }
        SystemCall do_process(core::NTypeLogger&) const override {
            return SystemCall::NOTHING ;
        }
        SystemCall do_process(const core::CharLogger&) const override {
            return SystemCall::NOTHING ;
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
        for(const auto& keyset : cmd) {
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
     * NOTE: It solves recursive mappings in key2keyset to a single mapping.
     * If a map is mapping to itself recursively, it will remove the map.
     */
    void solve_recursive_key2keyset_mapping(Key2KeysetMap& key2keyset_table) {
        for(unsigned char srckey = 1 ; srckey < 255 ; srckey ++) {
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
                    auto buf = key2keyset_table[itr->to_code()] ;
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
                auto& mapset = key2keyset_table[key.to_code()] ;
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
            const std::unordered_map<std::size_t, Map>& map_table,
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


    struct MapGate {
        std::unordered_map<std::size_t, std::vector<KeyLog>> logpool_{} ;
        LoggerParserManager mgr_{} ;
        Key2KeysetMap syncmap_{} ;


        void reconstruct(Mode mode) {
            std::unordered_map<std::size_t, Map> noremap_cmd2cmd{} ;
            std::unordered_map<std::size_t, Map> map_cmd2cmd{} ;
            std::array<Map, 256> map_key2keyset{} ;

            syncmap_.fill(KeySet{}) ;

            auto& maptable = core::MapTable::get_instance() ;
            auto maps = maptable.get_allmaps(mode) ;
            for(auto itr = std::make_move_iterator(maps.begin()) ;
                     itr != std::make_move_iterator(maps.end()) ; itr ++) {

                const auto& trigger_cmd = itr->trigger_command() ;
                const auto& target_cmd = itr->target_command() ;

                // Ignore mappings to the same command.
                if(trigger_cmd == target_cmd) {
                    continue ;
                }

                if(itr->is_noremap()) {
                    if(!bind::ref_global_funcs_bynames(itr->target_command_string())) {
                        noremap_cmd2cmd[itr->in_hash()] = std::move(*itr) ;
                    }
                }
                else { // remappable
                    // The key2keyset and key2key are mapped synchronously.
                    if(trigger_cmd.size() == 1 && trigger_cmd.front().size() == 1  // trigger is key?
                            && target_cmd.size() == 1) {                           // target is keyset?
                        auto trigger_key = trigger_cmd.front().front() ;
                        auto target_keyset = target_cmd.front() ;

                        syncmap_[trigger_key.to_code()] = KeySet(
                                target_keyset.begin(), target_keyset.end()) ;

                        map_key2keyset[trigger_key.to_code()] = std::move(*itr) ;
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
                        if(key.to_ascii()) {
                            keys.push_back(key) ;
                        }
                        else {
                            syskeys.push_back(key.to_physical()) ;
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
} // namespace


namespace vind
{
    namespace core
    {
        struct InputGate::Impl {
            HHOOK hook_ ;

            // Use bit-based optimization with std::vector<bool>.
            std::vector<bool> lowlevel_state_ ;
            std::vector<bool> real_state_ ;
            std::vector<bool> state_ ;  //Keyboard state win-vind understands.
            std::vector<bool> port_state_ ;

            std::array<std::chrono::system_clock::time_point, 256> timestamps_ ;

            bool absorb_state_ ;

            ModeArray<MapGate> mapgate_ ;

            std::queue<KeyLog> pool_ ;

            Impl()
            : hook_(nullptr),
              lowlevel_state_(256, false),
              real_state_(256, false),
              state_(256, false),
              port_state_(256, false),
              timestamps_(),
              absorb_state_(true),
              mapgate_(),
              pool_()
            {}

            ~Impl() noexcept = default ;
            Impl(const Impl&) = delete ;
            Impl& operator=(const Impl&) = delete ;
        } ;

        InputGate::InputGate()
        : pimpl(std::make_unique<Impl>())
        {}

        InputGate::~InputGate() noexcept {
            uninstall_hook() ;
        }

        InputGate& InputGate::get_instance() {
            static InputGate instance ;
            return instance ;
        }

        void InputGate::install_hook() {
            if(!pimpl->hook_) {
                pimpl->hook_ = SetWindowsHookEx(
                    WH_KEYBOARD_LL,
                    &InputGate::hook_proc,
                    NULL, 0
                ) ;
            }

            if(!pimpl->hook_) {
                throw RUNTIME_EXCEPT("KeyAbosorber's hook handle is null") ;
            }
        }

        void InputGate::uninstall_hook() noexcept {
            if(pimpl->hook_) {
                if(!UnhookWindowsHookEx(pimpl->hook_)) {
                    PRINT_ERROR("Cannot unhook LowLevelKeyboardProc") ;
                }
                pimpl->hook_ = nullptr ;
            }

            // prohibit to keep pressing after termination.
            for(unsigned char i = 1 ; i < 255 ; i ++) {
                if(pimpl->real_state_[i]) {
                    release_keystate(i) ;
                }
            }
        }

        void InputGate::reconstruct() {
            // Extraction of registered maps
            for(std::size_t mode = 0 ; mode < mode_num() ; mode ++) {
                pimpl->mapgate_[mode].reconstruct(static_cast<Mode>(mode)) ;
            }
        }

        LRESULT CALLBACK InputGate::hook_proc(int n_code, WPARAM w_param, LPARAM l_param) {
            if(n_code < HC_ACTION) { //not processed
                return CallNextHookEx(NULL, n_code, w_param, l_param) ;
            }

            auto& self = get_instance() ;

            auto kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(l_param) ;
            auto code = static_cast<unsigned char>(kbd->vkCode) ;
            if(!(kbd->flags & LLKHF_INJECTED)) {
                // The message is not generated with SendInput.
                auto state = !(w_param & KEYUP_MASK) ;

                self.pimpl->lowlevel_state_[code] = state ;
                self.pimpl->timestamps_[code] = std::chrono::system_clock::now() ;

                core::KeyCode keycode(code) ;
                if(auto repcode = keycode.to_representative()) {
                    if(self.map_syncstate(repcode, state) || \
                            self.map_syncstate(keycode, state)) {
                        return 1 ;
                    }

                    self.pimpl->real_state_[repcode.to_code()] = state ;
                    self.pimpl->state_[repcode.to_code()]      = state ;
                }
                else if(self.map_syncstate(keycode, state)) {
                    return 1 ;
                }

                self.pimpl->real_state_[code] = state ;
                self.pimpl->state_[code] = state ;
            }

            if(self.pimpl->port_state_[code]) {
                return CallNextHookEx(NULL, HC_ACTION, w_param, l_param) ;
            }
            if(self.pimpl->absorb_state_) {
                return 1 ;
            }
            return CallNextHookEx(NULL, HC_ACTION, w_param, l_param) ;
        }

        //
        // It makes the toggle keys behave just like any general keys. 
        // However, the current implementation has a small delay in releasing the state.
        // This is because LowLevelKeyboardProc (LLKP) and SetWindowsHookEx can only
        // capture the down event of the toggle key, not the up event, and the strange event cycle.
        // 
        // (1)
        // For example, we press CapsLock for 3 seconds. Then we will get the following signal in LLKP.
        //
        // Signal in LLKP:
        //
        //      ______________________
        // ____|               _ _ _ _
        //     0    1    2    3
        //
        // Thus, LLKP could not capture up event.
        //
        // (2)
        // So, let's use g_low_level_state to detect if the LLKP has been called or not.
        // Then, if the hook is not called for a while, it will release CapsLock.
        //
        // Signal out of LLKP:
        //
        //      __                          ___________________~~~~_____
        // ____|  |__(about 500ms)_________|                   ~~~~     |____
        //     0                                         1     ....  3 (has some delay)
        //
        // The toggle key is sending me a stroke instead of a key state (e.g. h.... hhhhhhh).
        // This means that there will be a buffer time of about 500ms after the first positive signal.
        // If you assign <CapsLock> to <Ctrl> and press <CapsLock-s>, 
        // as there is a high chance of collision during this buffer time, so it's so bad.
        //
        // (3)
        // As a result, I implemented to be released toggle keys
        // when LLKP has not been called for more than the buffer time.
        // This way, even if the key is actually released, if it is less than 500 ms,
        // it will continue to be pressed, causing a delay.
        //
        //
        // Unfortunately, for keymap and normal mode, the flow of key messages needs
        // to be stopped, and this can only be done using Hook.
        // Therefore, this is a challenging task.  If you have any ideas, we welcome pull requests.
        //
        void InputGate::refresh_toggle_state() {
            static util::IntervalTimer timer(5'000) ;
            if(!timer.is_passed()) {
                return ;
            }

            static auto toggles =[] {
                std::vector<core::KeyCode> buf ;
                for(unsigned char i = 1 ; i < 255 ; i ++) {
                    core::KeyCode k(i) ;
                    if(k.is_toggle()) {
                        buf.push_back(std::move(k)) ;
                    }
                }
                return buf ;
            }() ;

            for(auto k : toggles) {
                if(!pimpl->lowlevel_state_[k.to_code()]) {
                    continue ;
                }

                using namespace std::chrono ;
                if((system_clock::now() - pimpl->timestamps_[k.to_code()]) > 515ms) {
                    map_syncstate(k, false) ;
                    release_keystate(k) ;

                    pimpl->real_state_[k.to_code()] = false ;
                    pimpl->state_[k.to_code()] = false ;
                    pimpl->lowlevel_state_[k.to_code()] = false ;
                }
            }
        }

        bool InputGate::is_pressed(KeyCode keycode) noexcept {
            return pimpl->state_[keycode.to_code()] ;
        }
        bool InputGate::is_really_pressed(KeyCode keycode) noexcept {
            return pimpl->real_state_[keycode.to_code()] ;
        }

        KeyLog InputGate::pressed_list() {
            KeyLog::Data res{} ;
            for(unsigned char i = 1 ; i < 255 ; i ++) {
                if(is_pressed(i)) {
                    res.insert(i) ;
                }
            }
            return KeyLog(res) ;
        }

        bool InputGate::is_absorbed() noexcept {
            return pimpl->absorb_state_ ;
        }

        void InputGate::absorb() noexcept {
            pimpl->absorb_state_ = true ;
        }
        void InputGate::unabsorb() noexcept {
            pimpl->absorb_state_ = false ;
        }

        void InputGate::close_some_ports(
                std::initializer_list<KeyCode>&& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = true ;
            }
        }
        void InputGate::close_some_ports(
                std::initializer_list<KeyCode>::const_iterator begin,
                std::initializer_list<KeyCode>::const_iterator end) noexcept {
            for(auto itr = begin ; itr != end ; itr ++) {
                pimpl->port_state_[itr->to_code()] = false ;
            }
        }

        void InputGate::close_some_ports(
                std::vector<KeyCode>&& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = false ;
            }
        }
        void InputGate::close_some_ports(
                std::vector<KeyCode>::const_iterator begin,
                std::vector<KeyCode>::const_iterator end) noexcept {
            for(auto itr = begin ; itr != end ; itr ++) {
                pimpl->port_state_[itr->to_code()] = false ;
            }
        }

        void InputGate::close_some_ports(
                const KeyLog::Data& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = false ;
            }
        }

        void InputGate::close_port(KeyCode key) noexcept {
            pimpl->port_state_[key.to_code()] = false ;
        }

        void InputGate::close_all_ports() noexcept {
            std::fill(
                    pimpl->port_state_.begin(),
                    pimpl->port_state_.end(),
                    false) ;
        }

        void InputGate::close_all_ports_with_refresh() {
            // If this function is called by pressed button,
            // it has to send message "KEYUP" to OS (not absorbed).
            for(unsigned char i = 1 ; i < 255 ; i ++) {
                if(pimpl->state_[i]) {
                    // open a port to release the key state.
                    open_port(i) ;
                    release_keystate(i) ;
                }
            }

            std::fill(
                    pimpl->port_state_.begin(),
                    pimpl->port_state_.end(),
                    false) ;
        }

        void InputGate::open_some_ports(
                std::initializer_list<KeyCode>&& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = true ;
            }
        }
        void InputGate::open_some_ports(
                std::initializer_list<KeyCode>::const_iterator begin,
                std::initializer_list<KeyCode>::const_iterator end) noexcept {
            for(auto itr = begin ; itr != end ; itr ++) {
                pimpl->port_state_[itr->to_code()] = true ;
            }
        }

        void InputGate::open_some_ports(
                std::vector<KeyCode>&& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = true ;
            }
        }
        void InputGate::open_some_ports(
                std::vector<KeyCode>::const_iterator begin,
                std::vector<KeyCode>::const_iterator end) noexcept {
            for(auto itr = begin ; itr != end ; itr ++) {
                pimpl->port_state_[itr->to_code()] = true ;
            }
        }

        void InputGate::open_some_ports(
                const KeyLog::Data& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = true ;
            }
        }

        void InputGate::open_port(KeyCode key) noexcept {
            pimpl->port_state_[key.to_code()] = true ;
        }

        void InputGate::release_virtually(KeyCode key) noexcept {
            pimpl->state_[key.to_code()] = false ;
        }
        void InputGate::press_virtually(KeyCode key) noexcept {
            pimpl->state_[key.to_code()] = true ;
        }

        std::vector<KeyLog> InputGate::map_logger(
                const KeyLog& log,
                Mode mode) {
            auto midx = static_cast<int>(mode) ;

            auto& mgr = pimpl->mapgate_[midx].mgr_ ;

            auto parser = mgr.find_parser_with_transition(log) ;
            if(!parser) {
                mgr.reset_parser_states() ;
                return std::vector<KeyLog>{} ;
            }

            if(parser->is_accepted()) {
                auto func = parser->get_func() ;
                func->process() ;
                mgr.reset_parser_states() ;
                return pimpl->mapgate_[midx].logpool_[func->id()] ;

            }
            else if(parser->is_rejected_with_ready()) {
                mgr.reset_parser_states() ;
            }
            return std::vector<KeyLog>{} ;
        }

        bool InputGate::map_syncstate(
                KeyCode hook_key,
                bool press_sync_state,
                Mode mode) {
            auto midx = static_cast<int>(mode) ;
            auto target = pimpl->mapgate_[midx].syncmap_[hook_key.to_code()] ;
            if(target.empty()) {
                return false ;
            }

            if(press_sync_state) {
                open_some_ports(target.begin(), target.end()) ;
                press_keystate(target.begin(), target.end()) ;
                close_some_ports(target.begin(), target.end()) ;
            }
            else {
                open_some_ports(target.begin(), target.end()) ;
                release_keystate(target.begin(), target.end()) ;
                close_some_ports(target.begin(), target.end()) ;
            }
            return true ;
        }

        KeyLog InputGate::pop_log(Mode mode) {
            if(!pimpl->pool_.empty()) {
                auto log = std::move(pimpl->pool_.front()) ;
                pimpl->pool_.pop() ;
                return log ;
            }

            auto log = pressed_list() ;

            auto logs = map_logger(log, mode) ;
            if(logs.empty()) {
                return log ;
            }

            auto itr = std::make_move_iterator(logs.begin()) ;
            log = std::move(*itr) ;
            itr ++ ;

            //
            // To simulate the input, make a state transition with an empty log.
            // This is to make the logger recognize that it is a key release,
            // not a long pressing of the key.
            //
            while(itr != std::make_move_iterator(logs.end())) {
                pimpl->pool_.emplace() ;
                pimpl->pool_.push(std::move(*itr)) ;
                itr ++ ;
            }
            return log ;
        }
    } // namespace core
} // namespace vind


namespace
{
    bool is_pressed_actually(KeyCode key) noexcept {
        return GetAsyncKeyState(key.to_code()) & 0x8000 ;
    }
}

namespace vind
{
    namespace core
    {
        struct ScopedKey::Impl {
            INPUT in_ ;
            KeyCode key_ ;

            explicit Impl(KeyCode keycode)
            : in_(),
              key_(keycode)
            {
                in_.type       = INPUT_KEYBOARD ;
                in_.ki.wVk     = static_cast<WORD>(key_.to_code()) ;
                in_.ki.wScan   = static_cast<WORD>(MapVirtualKeyA(in_.ki.wVk, MAPVK_VK_TO_VSC)) ;
            }
        } ;

        ScopedKey::ScopedKey(KeyCode key)
        : pimpl(std::make_unique<Impl>(key))
        {}

        ScopedKey::~ScopedKey() noexcept {
            try {release() ;}
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }
        }

        ScopedKey::ScopedKey(ScopedKey&&)            = default ;
        ScopedKey& ScopedKey::operator=(ScopedKey&&) = default ;

        void ScopedKey::send_event(bool pressed) {
            pimpl->in_.ki.dwFlags = (pressed ? 0 : KEYEVENTF_KEYUP) | extended_key_flag(pimpl->in_.ki.wVk) ;
            if(!SendInput(1, &pimpl->in_, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }
        }

        void ScopedKey::press() {
            auto& igate = InputGate::get_instance() ;

            igate.open_port(pimpl->key_) ;
            send_event(true) ;
            igate.close_all_ports() ;
            if(!is_pressed_actually(pimpl->key_)) {
                throw RUNTIME_EXCEPT("You sent a key pressing event successfully, but the state of its key was not changed.") ;
            }
        }

        void ScopedKey::release() {
            auto& igate = InputGate::get_instance() ;

            igate.open_port(pimpl->key_) ;
            send_event(false) ;
            igate.close_all_ports() ;
            if(is_pressed_actually(pimpl->key_)) {
                throw RUNTIME_EXCEPT("You sent a key releasing event successfully, but the state of its key was not changed.") ;
            }
        }


        InstantKeyAbsorber::InstantKeyAbsorber()
        : flag_(false)
        {
            auto& igate = InputGate::get_instance() ;
            flag_ = igate.is_absorbed() ;
            igate.close_all_ports_with_refresh() ;
            igate.absorb() ;
        }

        InstantKeyAbsorber::~InstantKeyAbsorber() noexcept {
            if(!flag_) {
                InputGate::get_instance().unabsorb() ;
            }
        }
    } // namespace core
} // namespace vind
