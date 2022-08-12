#include "mapsolver.hpp"

#include "cmdmatcher.hpp"
#include "cmdparser_new.hpp"
#include "cmdunit.hpp"
#include "typeemu.hpp"

#include <algorithm>


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


    std::vector<CmdUnit> convert_from_pointer_cmdunit(
            const std::vector<CmdUnit::SPtr>& ptrcmd) {
        std::vector<CmdUnit> cmd{} ;
        for(auto& unitptr : ptrcmd) {
            cmd.push_back(*unitptr) ;
        }
        return cmd ;
    }

    void remove_triggered_map(
            std::vector<PreMap>& map_table,
            const std::vector<CmdUnit::SPtr>& remove_trigger) {
        std::vector<PreMap> new_map{} ;
        for(auto& map : map_table) {
            if(is_same_command(map.trigger_cmd, remove_trigger)) {
                new_map.push_back(std::move(map)) ;
            }
        }
        map_table.swap(new_map) ;
    }
}


namespace vind
{
    namespace core
    {
        struct MapSolver::Impl {
            std::vector<Map> deployed_{} ;
            std::vector<Map> default_noremap_{} ;
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

        void MapSolver::add_map(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) {
            auto trigger_cmd = parse_command(trigger_strcmd) ;
            auto target_ptrcmd = parse_command(target_strcmd) ;

            remove_triggered_map(pimpl->registered_noremap_, trigger_cmd) ;
            pimpl->registered_map_.emplace_back(trigger_cmd, target_ptrcmd) ;
        }

        void MapSolver::add_noremap(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) {
            auto trigger_cmd = parse_command(trigger_strcmd) ;
            auto target_ptrcmd = parse_command(target_strcmd) ;

            remove_triggered_map(pimpl->registered_map_, trigger_cmd) ;
            pimpl->registered_noremap_.emplace_back(trigger_cmd, target_ptrcmd) ;
        }

        void MapSolver::remove(const std::string& trigger_strcmd) {
            auto remove_cmd = parse_command(trigger_strcmd) ;

            remove_triggered_map(pimpl->registered_map_, remove_cmd) ;
            remove_triggered_map(pimpl->registered_noremap_, remove_cmd) ;
        }

        void MapSolver::solve() {
            pimpl->deployed_.clear() ;

            // noremap
            auto& refmap = pimpl->default_noremap_ ;
            for(auto& map : pimpl->registered_noremap_) {
                std::vector<CmdUnit::SPtr> solved_target ;
                for(auto& in_cmdunit : map.target_cmd) {
                    std::vector<int> acc_nums(refmap.size(), 0) ;
                    bool all_rejected = true ;
                    for(int i = 0 ; i < refmap.size() ; i ++) {
                        auto& mt = refmap[i].trigger_matcher ;
                        auto res = mt.update_state(*in_cmdunit) ;
                        if(mt.is_accepted()) {
                            acc_nums[i] = res ;
                        }
                        else if(!mt.is_rejected()) {
                            all_rejected = false ;
                        }
                    }

                    if(all_rejected) {
                        for(auto& map : refmap) {
                            map.trigger_matcher.reset_state() ;
                        }
                        continue ;
                    }

                    auto max_itr = std::max_element(acc_nums.begin(), acc_nums.end()) ;
                    if(*max_itr > 0) {
                        auto max_idx = std::distance(acc_nums.begin(), max_itr) ;
                        auto t_cmd = refmap[max_idx].target_cmd ;
                        solved_target.insert(
                            solved_target.begin(), t_cmd.begin(), t_cmd.end()) ;

                        for(auto& map : refmap) {
                            map.trigger_matcher.reset_state() ;
                        }
                    }
                }

                if(!solved_target.empty()) {
                    pimpl->deployed_.emplace_back(map.trigger_cmd, solved_target) ;
                }
            }


            // map

            // create matcher
            // solve recursively
        }

        void MapSolver::clear() {
            pimpl->deployed_.clear() ;
            pimpl->registered_noremap_.clear() ;
            pimpl->registered_map_.clear() ;
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
                in_cmdunit = std::move(*unit_ptr) ;
            }

            std::vector<int> acc_nums(pimpl->deployed_.size(), 0) ;
            bool all_rejected = true ;
            for(int i = 0 ; i < pimpl->deployed_.size() ; i ++) {
                auto& mt = pimpl->deployed_[i].trigger_matcher ;
                auto res = mt.update_state(in_cmdunit) ;
                if(mt.is_accepted()) {
                    acc_nums[i] = res ;
                }
                else if(!mt.is_rejected()) {
                    all_rejected = false ;
                }
            }

            if(all_rejected) {
                bool ignore = false ;
                if(pimpl->typeemu_) {
                    ignore = pimpl->typeemu_->ignore_if_syskey_only(in_cmdunit) ;
                }

                if(ignore) {
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

        bool MapSolver::map_syncstate(
                KeyCode hook_key,
                bool press_sync_state) {

            return true ;
        }
    }
}
