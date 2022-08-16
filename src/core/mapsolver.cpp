#include "mapsolver.hpp"

#include "cmdmatcher.hpp"
#include "cmdparser_new.hpp"
#include "cmdunit.hpp"
#include "errlogger.hpp"
#include "typeemu.hpp"

#include "util/debug.hpp"

#include <algorithm>
#include <sstream>
#include <string>


namespace
{
    using namespace vind::core ;
    struct Map {
        CmdMatcher trigger_matcher ;
        std::vector<CmdUnit::SPtr> target_cmd ;

        template <typename T1, typename T2>
        Map(T1&& trigger_command, T2&& target_command)
        : trigger_matcher(std::forward<T1>(trigger_command)),
          target_cmd(std::forward<T2>(target_command))
        {}
    } ;

    struct PreMap {
        std::vector<CmdUnit::SPtr> trigger_cmd ;
        std::vector<CmdUnit::SPtr> target_cmd ;

        template <typename T1, typename T2>
        PreMap(T1&& trigger_command, T2&& target_command)
        : trigger_cmd(std::forward<T1>(trigger_command)),
          target_cmd(std::forward<T2>(target_command))
        {}
    } ;

    template <typename T1, typename T2>
    bool is_same_command(T1&& cmd1, T2&& cmd2) {
        for(const auto& unit1 : cmd1) {
            for(const auto& unit2 : cmd2) {
                if(*unit1 != *unit2) {
                    return false ;
                }
            }
        }
        return true ;
    }

    bool remove_triggered_map(
            std::vector<PreMap>& map_table,
            const std::vector<CmdUnit::SPtr>& remove_trigger) {
        std::vector<PreMap> new_map{} ;
        for(auto& map : map_table) {
            if(!is_same_command(map.trigger_cmd, remove_trigger)) {
                new_map.push_back(std::move(map)) ;
            }
        }
        map_table.swap(new_map) ;
        return new_map.size() != map_table.size() ;
    }

    std::vector<CmdUnit::SPtr> solve_mapping_recursive_impl(
            const PreMap& premap,
            std::vector<Map>& refmap_table,
            bool recursively=false) {

        std::vector<CmdUnit::SPtr> solved_target ;
        if(premap.target_cmd.empty()) {
            return solved_target ;
        }

        for(auto& map : refmap_table) {
            map.trigger_matcher.reset_state() ;
        }

        for(const auto& in_cmdunit : premap.target_cmd) {
            std::vector<int> acc_nums(refmap_table.size(), 0) ;
            bool all_rejected = true ;
            for(int i = 0 ; i < refmap_table.size() ; i ++) {
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
                for(auto& map : refmap_table) {
                    map.trigger_matcher.reset_state() ;
                }
                solved_target.push_back(in_cmdunit) ;
                continue ;
            }

            auto max_itr = std::max_element(acc_nums.begin(), acc_nums.end()) ;
            if(*max_itr > 0) {
                auto max_idx = std::distance(acc_nums.begin(), max_itr) ;
                auto t_cmd = refmap_table[max_idx].target_cmd ;

                if(!recursively) {
                    solved_target.insert(
                        solved_target.end(), t_cmd.begin(), t_cmd.end()) ;
                }
                else if(!is_same_command(premap.trigger_cmd, t_cmd)) {
                    PreMap solved_premap{premap.trigger_cmd, t_cmd} ;
                    auto solved_subcmd = solve_mapping_recursive_impl(
                        solved_premap, refmap_table, true) ;
                    if(solved_subcmd.empty()) {
                        solved_target.insert(
                            solved_target.end(), t_cmd.begin(), t_cmd.end()) ;
                    }
                    else {
                        solved_target.insert(
                            solved_target.end(),
                            solved_subcmd.begin(), solved_subcmd.end()) ;
                    }
                }

                for(auto& map : refmap_table) {
                    map.trigger_matcher.reset_state() ;
                }
            }
        }

        return solved_target ;
    }

    std::vector<CmdUnit::SPtr> solve_mapping(
            const PreMap& premap,
            std::vector<Map>& refmap_table,
            bool recursively=false) {
        if(is_same_command(premap.trigger_cmd, premap.target_cmd)) {
            return {} ;
        }
        return solve_mapping_recursive_impl(premap, refmap_table, recursively) ;
    }
}


namespace vind
{
    namespace core
    {
        struct MapSolver::Impl {
            std::vector<Map> deployed_{} ;
            std::vector<Map> default_{} ;
            std::vector<PreMap> registered_default_{} ;
            std::vector<PreMap> registered_noremap_{} ;
            std::vector<PreMap> registered_map_{} ;
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

        void MapSolver::add_default(
            const std::string& trigger_strcmd,
            const std::string& target_strcmd) {
            auto trigger_cmd = parse_command(trigger_strcmd) ;
            auto target_ptrcmd = parse_command(target_strcmd) ;

            remove_triggered_map(pimpl->registered_default_, trigger_cmd) ;
            pimpl->registered_default_.emplace_back(trigger_cmd, target_ptrcmd) ;
        }

        void MapSolver::add_map(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) {
            auto trigger_cmd = parse_command(trigger_strcmd) ;
            auto target_ptrcmd = parse_command(target_strcmd) ;

            remove_triggered_map(pimpl->registered_map_, trigger_cmd) ;
            remove_triggered_map(pimpl->registered_noremap_, trigger_cmd) ;
            pimpl->registered_map_.emplace_back(trigger_cmd, target_ptrcmd) ;
        }

        void MapSolver::add_noremap(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) {
            auto trigger_cmd = parse_command(trigger_strcmd) ;
            auto target_ptrcmd = parse_command(target_strcmd) ;

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

        void MapSolver::deploy() {
            pimpl->deployed_.clear() ;

            std::vector<Map> solved_maps{} ;
            solved_maps.reserve(
                pimpl->registered_noremap_.size() + pimpl->registered_map_.size()) ;

            std::vector<PreMap> tmp_noremap{} ;
            tmp_noremap.reserve(
                pimpl->default_.size() + pimpl->registered_noremap_.size()) ;

            for(const auto& map : pimpl->default_) {
                tmp_noremap.emplace_back(
                    map.trigger_matcher.get_command(), map.target_cmd) ;
            }

            for(const auto& map : pimpl->registered_noremap_) {
                remove_triggered_map(tmp_noremap, map.trigger_cmd) ;
                tmp_noremap.emplace_back(map.trigger_cmd, map.target_cmd) ;
            }

            for(const auto& premap : tmp_noremap) {
                auto solved_target = solve_mapping(premap, pimpl->default_, false) ;
                if(!solved_target.empty()) {
                    solved_maps.emplace_back(premap.trigger_cmd, solved_target) ;
                }
            }

            auto tmp_maps = solved_maps ;
            for(const auto& premap : pimpl->registered_map_) {
                tmp_maps.emplace_back(premap.trigger_cmd, premap.target_cmd) ;
            }

            for(const auto& premap : pimpl->registered_map_) {
                auto solved_target = solve_mapping(premap, tmp_maps, true) ;
                if(!solved_target.empty()) {
                    solved_maps.emplace_back(premap.trigger_cmd, solved_target) ;
                }
            }


            for(auto& map :solved_maps) {
                if(map.trigger_matcher.get_command().size() == 1 &&
                        map.trigger_matcher.get_command()[0]->size() == 1 &&
                        map.target_cmd.size() == 1) {
                    // key2keyset mapping
                    // register_inputgate

                    // for debug -----------
                    //
                    pimpl->deployed_.push_back(std::move(map)) ;
                    ///
                    ///---------------------
                }
                else {
                    pimpl->deployed_.push_back(std::move(map)) ;
                }
            }
        }

        void MapSolver::deploy_default(bool solve) {
            std::vector<Map> tmp_maps ;
            tmp_maps.reserve(pimpl->registered_default_.size()) ;

            for(const auto& premap : pimpl->registered_default_) {
                tmp_maps.emplace_back(premap.trigger_cmd, premap.target_cmd) ;
            }

            if(!solve) {
                pimpl->default_ = tmp_maps ;
                return ;
            }

            pimpl->default_.clear() ;
            for(auto& premap : pimpl->registered_default_) {
                auto solved_target = solve_mapping(premap, tmp_maps, true) ;
                if(!solved_target.empty()) {
                    pimpl->default_.emplace_back(premap.trigger_cmd, solved_target) ;
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

        void MapSolver::backward_state(std::size_t n) {
            for(auto& map : pimpl->deployed_) {
                map.trigger_matcher.backward_state(n) ;
            }
        }

        void MapSolver::reset_state() {
            for(auto& map : pimpl->deployed_) {
                map.trigger_matcher.reset_state() ;
            }
        }

        // If the trigger command is matched, it return true.
        bool MapSolver::map_command_to(
                const CmdUnit& raw_cmdunit,
                std::vector<CmdUnit::SPtr>& target_cmd,
                unsigned int& count) {

            CmdUnit in_cmdunit{} ;
            if(pimpl->typeemu_) {
                auto unit_ptr = pimpl->typeemu_->lowlevel_to_typing(raw_cmdunit) ;
                if(!unit_ptr) {
                    return false ;
                }
                in_cmdunit = *unit_ptr ;
            }
            else {
                in_cmdunit = raw_cmdunit ;
            }

            std::vector<int> acc_nums(pimpl->deployed_.size(), 0) ;
            bool all_rejected = true ;
            for(int i = 0 ; i < pimpl->deployed_.size() ; i ++) {
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
                else {
                    for(auto& map : pimpl->deployed_) {
                        map.trigger_matcher.reset_state() ;
                    }
                }
                return false ;
            }

            auto max_itr = std::max_element(acc_nums.begin(), acc_nums.end()) ;
            if(*max_itr > 0) {
                auto max_idx = std::distance(acc_nums.begin(), max_itr) ;
                count = 1 ;
                target_cmd = pimpl->deployed_[max_idx].target_cmd ;

                for(auto& map : pimpl->deployed_) {
                    map.trigger_matcher.reset_state() ;
                }

                return true ;
            }
            return false ;
        }

        std::vector<std::vector<CmdUnit::SPtr>> MapSolver::get_trigger_commands() const {
            std::vector<std::vector<CmdUnit::SPtr>> tmp{} ;
            for(const auto& map : pimpl->deployed_) {
                tmp.push_back(map.trigger_matcher.get_command()) ;
            }
            return tmp ;
        }

        std::vector<std::vector<CmdUnit::SPtr>> MapSolver::get_target_commands() const {
            std::vector<std::vector<CmdUnit::SPtr>> tmp{} ;
            for(const auto& map : pimpl->deployed_) {
                tmp.emplace_back(map.target_cmd) ;
            }
            return tmp ;
        }
    }
}
