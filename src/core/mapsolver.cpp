#include "mapsolver.hpp"

#include "cmdmatcher.hpp"
#include "cmdparser.hpp"
#include "cmdunit.hpp"
#include "errlogger.hpp"
#include "inputgate.hpp"
#include "typeemu.hpp"

#include "util/debug.hpp"

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>


namespace
{
    using namespace vind::core ;
    using LinkedCmd = std::vector<std::vector<CmdUnit::SPtr>> ;

    struct Map {
        CmdMatcher trigger_matcher ;

        // Register in link format to distinguish between macros and mappings.
        // For example, when `gmap :!notepad<cr>` is written, it is noto
        // intuitive if `o` is replaced by `<click_left>` in prior resolution,
        // because we want to keep the key before the replacement.
        LinkedCmd mapped_cmd ;

        template <typename T1, typename T2>
        Map(
            T1&& trigger_command,
            T2&& linked_command)
        : trigger_matcher(std::forward<T1>(trigger_command)),
          mapped_cmd(std::forward<T2>(linked_command))
        {}
    } ;

    struct DraftMap {
        std::vector<CmdUnit::SPtr> trigger_cmd ;
        std::vector<CmdUnit::SPtr> target_cmd ;

        template <typename T1, typename T2>
        DraftMap(T1&& trigger_command, T2&& target_command)
        : trigger_cmd(std::forward<T1>(trigger_command)),
          target_cmd(std::forward<T2>(target_command))
        {}
    } ;

    bool is_same_command(
            const std::vector<CmdUnit::SPtr>& lhs,
            const std::vector<CmdUnit::SPtr>& rhs) {
        if(lhs.size() != rhs.size()) {
            return false ;
        }
        for(std::size_t i = 0 ; i < lhs.size() ; i ++) {
            if(*(lhs[i]) != *(rhs[i])) {
                return false ;
            }
        }
        return true ;
    }

    /**
     * It removes the map having the same trigger command as
     * remove_trigger from the list of the map.
     * If it removed, returns true, otherwise false.
     */
    bool remove_triggered_map(
            std::vector<DraftMap>& map_table,
            const std::vector<CmdUnit::SPtr>& remove_trigger) {
        std::vector<DraftMap> new_map{} ;
        for(auto& map : map_table) {
            if(!is_same_command(map.trigger_cmd, remove_trigger)) {
                new_map.push_back(std::move(map)) ;
            }
        }
        map_table.swap(new_map) ;
        return new_map.size() != map_table.size() ;
    }

    bool remove_triggered_map(
            std::vector<Map>& map_table,
            const std::vector<CmdUnit::SPtr>& remove_trigger) {
        std::vector<Map> new_map{} ;
        for(auto& map : map_table) {
            if(!is_same_command(
                    map.trigger_matcher.get_command(), remove_trigger)) {
                new_map.push_back(std::move(map)) ;
            }
        }
        map_table.swap(new_map) ;
        return new_map.size() != map_table.size() ;
    }

    /**
     * It solves multiple remapping to single mapping recursively.
     * This function is designed to call from only solve_mapping function.
     */
    std::vector<CmdUnit::SPtr> solve_mapping_recursive_impl(
            const DraftMap& draft_map,
            std::vector<Map>& refmap_table,
            bool recursively=false) {
        std::vector<CmdUnit::SPtr> solved_target ;
        for(auto& map : refmap_table) {
            map.trigger_matcher.reset_state() ;
        }

        std::vector<CmdUnit::SPtr> subcmd_queue{} ;
        for(const auto& in_cmdunit : draft_map.target_cmd) {
            // If the mapping like `gmap t {hello}` could result in
            // `o` being `click_left`. However, the external mapping
            // take precedence over functional mappings because
            // the user likely want to input the keys.
            if(std::dynamic_pointer_cast<ExternalCmdUnit>(in_cmdunit)) {
                subcmd_queue.push_back(in_cmdunit) ;
                solved_target.insert(
                    solved_target.end(),
                    subcmd_queue.begin(), subcmd_queue.end()) ;
                subcmd_queue.clear() ;
                continue ;
            }

            // Saves the matched number of each matcher.
            // Then, gives high priority to the matcher
            // having the maximum matched number.
            std::vector<int> acc_nums(refmap_table.size(), 0) ;
            bool all_rejected = true ;
            for(std::size_t i = 0 ; i < refmap_table.size() ; i ++) {
                auto& mt = refmap_table[i].trigger_matcher ;
                auto res = mt.update_state(*in_cmdunit) ;
                if(mt.is_accepted()) {
                    acc_nums[i] = res ;
                }
                if(!mt.is_rejected()) {
                    all_rejected = false ;
                }
            }

            if(all_rejected) {
                // If all matchers do not match with the target sub-command
                // of draft_map, the target sub command keeps as is.
                for(auto& map : refmap_table) {
                    map.trigger_matcher.reset_state() ;
                }
                subcmd_queue.push_back(in_cmdunit) ;
                solved_target.insert(
                    solved_target.end(),
                    subcmd_queue.begin(), subcmd_queue.end()) ;
                subcmd_queue.clear() ;
                continue ;
            }

            auto max_itr = std::max_element(acc_nums.begin(), acc_nums.end()) ;
            if(*max_itr == 0) {
                // Any matchers does not accept, but matching.
                subcmd_queue.push_back(in_cmdunit) ;
                continue ;
            }

            // Some matchers are accepted.
            auto max_idx = std::distance(acc_nums.begin(), max_itr) ;
            auto t_cmd = refmap_table[max_idx].mapped_cmd.back() ;
            if(!t_cmd.empty()) {
                if(!recursively) {
                    solved_target.insert(
                        solved_target.end(), t_cmd.begin(), t_cmd.end()) ;
                }
                else if(!is_same_command(draft_map.trigger_cmd, t_cmd)) {
                    auto solved_subcmd = solve_mapping_recursive_impl(
                            DraftMap{draft_map.trigger_cmd, t_cmd},
                            refmap_table, true) ;
                    solved_target.insert(
                        solved_target.end(),
                        solved_subcmd.begin(), solved_subcmd.end()) ;
                }
            }

            // Make the matchers reset for the next matching.
            for(auto& map : refmap_table) {
                map.trigger_matcher.reset_state() ;
            }
            subcmd_queue.clear() ;
        }

        // Matching the target command, the loop is breaked.
        solved_target.insert(
            solved_target.end(),
            subcmd_queue.begin(), subcmd_queue.end()) ;

        return solved_target ;
    }


    /*
     * It solves multiple remapping to single mapping.
     *
     * e.g.
     * Before
     *   map b h
     *   map <s-f> ub
     *   map <c-y> <s-f><c-v>
     *
     * After
     *   map b h
     *   map <s-f> uh
     *   map <c-y> uh<c-v>
     *
     * @param[in] (draft_map) The target map to solve remapping.
     * @param[in] (refmap_table) The reference map table to search a command triggered with the target command of the target map.
     * @param[in] (recursively) If it is false, it solves only once, otherwise, it solves recursively like the above example.
     * @return The target command solved from draft_map.target_cmd.
     *
     */
    std::vector<CmdUnit::SPtr> solve_mapping(
            const DraftMap& draft_map,
            std::vector<Map>& refmap_table,
            bool recursively=false) {
        // We allow the external key2key self-mapping to disable key absorbing.
        // (e.g. map <alt> {<alt>})
        if(draft_map.trigger_cmd.size() == 1 \
                && draft_map.target_cmd.size() == 1 \
                && std::dynamic_pointer_cast<ExternalCmdUnit>(draft_map.target_cmd[0])) {
            return draft_map.target_cmd ;
        }

        // However, other self-mapping (e.g. map <s-c> <s-c>) is invalid.
        if(is_same_command(draft_map.trigger_cmd, draft_map.target_cmd)) {
            return {} ;
        }
        return solve_mapping_recursive_impl(draft_map, refmap_table, recursively) ;
    }
}


namespace vind
{
    namespace core
    {
        struct MapSolver::Impl {
            std::vector<Map> deployed_{} ;
            std::vector<Map> default_{} ;
            std::vector<DraftMap> registered_default_{} ;
            std::vector<DraftMap> registered_noremap_{} ;
            std::vector<DraftMap> registered_map_{} ;
            std::unique_ptr<TypingEmulator> typeemu_{nullptr} ;
        } ;

        MapSolver::MapSolver(bool enable_typing_emulator)
        : pimpl(std::make_unique<Impl>())
        {
            if(enable_typing_emulator) {
                pimpl->typeemu_ = std::make_unique<TypingEmulator>() ;
            }
        }

        MapSolver::~MapSolver() noexcept = default ;

        MapSolver::MapSolver(MapSolver&&) = default ;
        MapSolver& MapSolver::operator=(MapSolver&&) = default ;

        void MapSolver::copy_deployment_from(const MapSolver& rhs) {
            pimpl->deployed_ = rhs.pimpl->deployed_ ;
            pimpl->typeemu_ = std::make_unique<TypingEmulator>(*(rhs.pimpl->typeemu_)) ;
        }

        void MapSolver::add_default(
            const std::string& trigger_strcmd,
            const std::string& target_strcmd) {
            auto trigger_cmd = parse_command(trigger_strcmd) ;
            auto target_ptrcmd = parse_command(target_strcmd) ;

            // Remove the same trigger to overwrite map.
            remove_triggered_map(pimpl->registered_default_, trigger_cmd) ;
            pimpl->registered_default_.emplace_back(trigger_cmd, target_ptrcmd) ;
        }

        void MapSolver::add_map(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) {
            auto trigger_cmd = parse_command(trigger_strcmd) ;
            auto target_ptrcmd = parse_command(target_strcmd) ;

            // Remove the same trigger to overwrite map.
            remove_triggered_map(pimpl->registered_map_, trigger_cmd) ;
            remove_triggered_map(pimpl->registered_noremap_, trigger_cmd) ;
            pimpl->registered_map_.emplace_back(trigger_cmd, target_ptrcmd) ;
        }

        void MapSolver::add_noremap(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) {
            auto trigger_cmd = parse_command(trigger_strcmd) ;
            auto target_ptrcmd = parse_command(target_strcmd) ;

            // Remove the same trigger to overwrite map.
            remove_triggered_map(pimpl->registered_map_, trigger_cmd) ;
            remove_triggered_map(pimpl->registered_noremap_, trigger_cmd) ;
            pimpl->registered_noremap_.emplace_back(trigger_cmd, target_ptrcmd) ;
        }

        bool MapSolver::remove(const std::string& trigger_strcmd) {
            auto remove_cmd = parse_command(trigger_strcmd) ;
            return remove_triggered_map(pimpl->registered_map_, remove_cmd) ||
                   remove_triggered_map(pimpl->registered_noremap_, remove_cmd) ;
        }

        bool MapSolver::remove_default(const std::string& trigger_strcmd) {
            auto remove_cmd = parse_command(trigger_strcmd) ;
            return remove_triggered_map(pimpl->registered_default_, remove_cmd) ;
        }

        void MapSolver::deploy(Mode mode) {
            pimpl->deployed_.clear() ;

            if(mode != Mode::UNDEFINED) {
                InputGate::get_instance().clear_hotkeys(mode) ;
            }

            std::vector<Map> solved_maps{} ;
            solved_maps.reserve(
                pimpl->registered_noremap_.size() + pimpl->registered_map_.size()) ;

            std::vector<DraftMap> tmp_noremap{} ;
            tmp_noremap.reserve(
                pimpl->default_.size() + pimpl->registered_noremap_.size()) ;

            // Initialize with default noremap.
            for(const auto& map : pimpl->default_) {
                tmp_noremap.emplace_back(
                    map.trigger_matcher.get_command(),
                    map.mapped_cmd.back()) ;
            }

            // Add and overwrite map with user-defined map.
            for(const auto& draft_map : pimpl->registered_noremap_) {
                remove_triggered_map(tmp_noremap, draft_map.trigger_cmd) ;
                tmp_noremap.emplace_back(draft_map.trigger_cmd, draft_map.target_cmd) ;
            }

            for(const auto& draft_map : tmp_noremap) {
                auto solved_target = solve_mapping(draft_map, pimpl->default_, false) ;
                if(!solved_target.empty()) {
                    auto linked_cmd = {draft_map.target_cmd, solved_target} ;
                    solved_maps.emplace_back(
                        draft_map.trigger_cmd, std::move(linked_cmd)) ;
                }
            }

            // Initialize with default noremap and solved noremap.
            // Each matcher of tmp_maps is used to solve the remapping.
            auto tmp_maps = solved_maps ;
            for(const auto& draft_map : pimpl->registered_map_) {
                // To remove the duplicate trigger command from default map, 
                // By the way, noremap and map are disjoint.
                remove_triggered_map(tmp_maps, draft_map.trigger_cmd) ;
                auto linked_cmd = {draft_map.target_cmd} ;
                tmp_maps.emplace_back(
                    draft_map.trigger_cmd, std::move(linked_cmd)) ;
            }

            for(const auto& draft_map : pimpl->registered_map_) {
                auto solved_target = solve_mapping(draft_map, tmp_maps, true) ;
                if(!solved_target.empty()) {
                    remove_triggered_map(solved_maps, draft_map.trigger_cmd) ;
                    auto linked_cmd = {draft_map.target_cmd, solved_target} ;
                    solved_maps.emplace_back(
                        draft_map.trigger_cmd, std::move(linked_cmd)) ;
                }
            }

            for(auto& map : solved_maps) {
                auto& trigger_cmd = map.trigger_matcher.get_command() ;
                if(trigger_cmd.size() == 1 && trigger_cmd[0]->size() == 1 &&
                        map.mapped_cmd.back().size() == 1) {
                    // key2keyset mapping
                    auto& tgtunit = map.mapped_cmd.back()[0] ;
                    if(mode != Mode::UNDEFINED \
                            && std::dynamic_pointer_cast<ExternalCmdUnit>(tgtunit)) {
                        // For the external command unit, applies low-level mapping.
                        InputGate::get_instance().register_hotkey(*trigger_cmd[0]->begin(), *tgtunit, mode) ;
                    }
                    else {
                        pimpl->deployed_.push_back(std::move(map)) ;
                    }
                }
                else {
                    pimpl->deployed_.push_back(std::move(map)) ;
                }
            }
        }

        void MapSolver::deploy_default(bool solve) {
            std::vector<Map> tmp_maps ;
            tmp_maps.reserve(pimpl->registered_default_.size()) ;

            for(const auto& draft_map : pimpl->registered_default_) {
                auto linked_cmd = {draft_map.target_cmd} ;
                tmp_maps.emplace_back(
                    draft_map.trigger_cmd, std::move(linked_cmd)) ;
            }

            if(!solve) {
                pimpl->default_ = tmp_maps ;
                return ;
            }

            pimpl->default_.clear() ;
            for(auto& draft_map : pimpl->registered_default_) {
                auto solved_target = solve_mapping(draft_map, tmp_maps, true) ;
                if(!solved_target.empty()) {
                    auto linked_cmd = {solved_target} ;
                    pimpl->default_.emplace_back(
                        draft_map.trigger_cmd, std::move(linked_cmd)) ;
                }
            }
        }

        void MapSolver::clear() {
            pimpl->registered_noremap_.clear() ;
            pimpl->registered_map_.clear() ;
        }

        void MapSolver::clear_default() {
            pimpl->registered_default_.clear() ;
        }

        void MapSolver::backward_state(int n) {
            for(auto& map : pimpl->deployed_) {
                map.trigger_matcher.backward_state(n) ;
            }
        }

        void MapSolver::reset_state() {
            for(auto& map : pimpl->deployed_) {
                map.trigger_matcher.reset_state() ;
            }
        }

        std::vector<CmdUnit::SPtr>
        MapSolver::map_command_from(
                const CmdUnit& raw_cmdunit,
                bool auto_reset) {
            // Converts the sequentially duplicated low-level input
            // to the formatted command unit as if typing.
            CmdUnit in_cmdunit{} ;
            if(pimpl->typeemu_) {
                auto unit_ptr = pimpl->typeemu_->lowlevel_to_typing(raw_cmdunit) ;
                if(!unit_ptr) {
                    return {} ;
                }
                in_cmdunit = *unit_ptr ;
            }
            else {
                in_cmdunit = raw_cmdunit ;
            }

            // Saves the matched number of each matcher. Then, gives high
            // priority to the matcher having the maximum matched number.
            std::vector<int> acc_nums(pimpl->deployed_.size(), 0) ;
            bool all_rejected = true ;
            for(std::size_t i = 0 ; i < pimpl->deployed_.size() ; i ++) {
                auto& mt = pimpl->deployed_[i].trigger_matcher ;
                auto res = mt.update_state(in_cmdunit) ;
                if(mt.is_accepted()) {
                    acc_nums[i] = res ;
                }
                if(!mt.is_rejected()) {
                    all_rejected = false ;
                }
            }

            if(all_rejected) {
                // When all matchers rejected with a command unit
                // having only system keycodes, the rejection is
                // unwound to match the potential matchers next time.
                //
                // e.g.
                //  In order to input <s-f><s-f>, we must type like the following.
                //
                //  <shift>  ->  <shift-f>  ->  <shift>  ->  <shift-f>
                //
                //  Therefore, should not reject with the internal <shift>.
                //
                bool only_syskey = true ;
                for(const auto& key : in_cmdunit) {
                    if(!key.is_major_system()){
                        only_syskey = false ;
                        break ;
                    }
                }
                if(only_syskey) {
                    for(auto& map : pimpl->deployed_) {
                        map.trigger_matcher.backward_state(1) ;
                    }
                }
                else if(auto_reset) {
                    for(auto& map : pimpl->deployed_) {
                        map.trigger_matcher.reset_state() ;
                    }
                }
                return {} ;
            }

            auto max_itr = std::max_element(acc_nums.begin(), acc_nums.end()) ;
            if(*max_itr == 0) {
                return {} ;
            }

            auto max_idx = std::distance(acc_nums.begin(), max_itr) ;

            // Make the matchers reset for the next matching.
            if(auto_reset) {
                for(auto& map : pimpl->deployed_) {
                    map.trigger_matcher.reset_state() ;
                }
            }

            return pimpl->deployed_[max_idx].mapped_cmd.back() ;
        }

        std::vector<std::vector<CmdUnit::SPtr>>
        MapSolver::get_trigger_commands() const {
            std::vector<std::vector<CmdUnit::SPtr>> tmp{} ;
            for(const auto& map : pimpl->deployed_) {
                tmp.push_back(map.trigger_matcher.get_command()) ;
            }
            return tmp ;
        }

        std::vector<std::vector<CmdUnit::SPtr>>
        MapSolver::get_target_commands() const {
            std::vector<std::vector<CmdUnit::SPtr>> tmp{} ;
            for(const auto& map : pimpl->deployed_) {
                tmp.emplace_back(map.mapped_cmd.back()) ;
            }
            return tmp ;
        }

        bool MapSolver::is_matching_any() const noexcept {
            for(const auto& map : pimpl->deployed_) {
                if(map.trigger_matcher.history_size() > 0) {
                    return true ;
                }
            }
            return false ;
        }

        bool MapSolver::is_accepted_any() const noexcept {
            for(const auto& map : pimpl->deployed_) {
                if(map.trigger_matcher.is_accepted()) {
                    return true ;
                }
            }
            return false ;
        }

        bool MapSolver::is_rejected_any() const noexcept {
            for(const auto& map : pimpl->deployed_) {
                if(map.trigger_matcher.is_rejected()) {
                    return true ;
                }
            }
            return false ;
        }

        bool MapSolver::is_rejected_all() const noexcept {
            bool flag = true ;
            for(const auto& map : pimpl->deployed_) {
                if(!map.trigger_matcher.is_rejected()) {
                    flag = false ;
                    break ;
                }
            }
            return flag ;
        }
    }
}
