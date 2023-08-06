#include "autocmd.hpp"

#include "cmdparser.hpp"
#include "cmdunit.hpp"
#include "util/winwrap.hpp"

#include <array>
#include <memory>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <vector>

#include <tlhelp32.h>


namespace
{
    using namespace vind ;

    class AutoPattern {
    private:
        std::string pat_ ;
        std::regex re_ ;

    public:
        explicit AutoPattern()
        : AutoPattern("")
        {}

        explicit AutoPattern(const std::string& pat)
        : pat_(pat),
          re_(pat)
        {}

        explicit AutoPattern(std::string&& pat)
        : pat_(std::move(pat)),
          re_(pat_)
        {}

        // Match a query with the pattern and returns the match score.
        // The higher score denotes more specified matching.
        int match(const std::string& query) {
            if(pat_.length() == 1 && pat_.front() == '*') {
                return 1 ;
            }

            if(std::regex_match(query, re_)) {
                return 2 ;
            }

            return 0 ;
        }

        const std::string& get() const noexcept {
            return pat_ ;
        }
    } ;

    using SequentialCmd = std::vector<std::vector<core::CmdUnit::SPtr>> ;

    class AutoEvent {
    private:
        std::vector<AutoPattern> pats_ ;
        std::vector<SequentialCmd> seqcmds_ ;
        SequentialCmd empty_ ;

        int get_pat_index(const std::string& query) {
            for(int i = 0 ; i < pats_.size() ; i ++) {
                if(pats_[i].get() == query) {
                    return i ;
                }
            }
            return -1 ;
        }

    public:
        explicit AutoEvent()
        : pats_(),
          seqcmds_(),
          empty_()
        {}

        template <typename T1, typename T2>
        void add_pattern_cmd(T1&& pat, T2&& cmd) {
            auto idx = get_pat_index(pat) ;
            if(idx < 0) {
                pats_.emplace_back(std::forward<T1>(pat)) ;
                seqcmds_.push_back({std::forward<T2>(cmd)}) ;
            }
            else {
                seqcmds_[idx].push_back(std::forward<T2>(cmd)) ;
            }
        }

        int match_pattern(const std::string& query) {
            int highest_index = -1 ;
            int highest_score = 0 ;
            for(int i = 0 ; i < pats_.size() ; i ++) {
                auto score = pats_[i].match(query) ;
                if(highest_score < score) {
                    highest_index = i ;
                }
            }

            return highest_index ;
        }

        void call_sequential_cmd(int patidx) {
            for(const auto& cmd : seqcmds_[patidx]) {
                std::cout << "call: " << cmd << std::endl ;
                for(const auto& unit : cmd) {
                    unit->execute(1) ;
                }
            }
        }
    } ;
}

namespace vind
{
    namespace core
    {
        AutoCmdEvent get_autocmd_event(const std::string& event_name) {
            static std::unordered_map<std::string, AutoCmdEvent> names {
                {"WinEnter",        AutoCmdEvent::WIN_ENTER},
                {"WinLeave",        AutoCmdEvent::WIN_LEAVE},
                {"GUINormalEnter",  AutoCmdEvent::GUI_NORMAL_ENTER},
                {"GUINormalLeave",  AutoCmdEvent::GUI_NORMAL_LEAVE},
                {"GUIVisualEnter",  AutoCmdEvent::GUI_VISUAL_ENTER},
                {"GUIVisualLeave",  AutoCmdEvent::GUI_VISUAL_LEAVE},
                {"EdiNormalEnter",  AutoCmdEvent::EDI_NORMAL_ENTER},
                {"EdiNormalLeave",  AutoCmdEvent::EDI_NORMAL_LEAVE},
                {"EdiVisualEnter",  AutoCmdEvent::EDI_VISUAL_ENTER},
                {"EdiVisualLeave",  AutoCmdEvent::EDI_VISUAL_LEAVE},
                {"InsertEnter",     AutoCmdEvent::INSERT_ENTER},
                {"InsertLeave",     AutoCmdEvent::INSERT_LEAVE},
                {"ResidentEnter",   AutoCmdEvent::RESIDENT_ENTER},
                {"ResidentLeave",   AutoCmdEvent::RESIDENT_LEAVE},
                {"CommandEnter",    AutoCmdEvent::COMMAND_ENTER},
                {"CommandLeave",    AutoCmdEvent::COMMAND_LEAVE}
            } ;
            return names.at(event_name) ;
        }

        struct AutoCmd::Impl {
            std::array<AutoEvent, static_cast<int>(AutoCmdEvent::EVENT_NUM)> events_ ;

            explicit Impl()
            : events_()
            {}
        } ;

        AutoCmd::AutoCmd()
        : pimpl(std::make_unique<Impl>())
        {}

        AutoCmd::~AutoCmd() noexcept = default ;

        AutoCmd& AutoCmd::get_instance() {
            static AutoCmd instance{} ;
            return instance ;
        }

        void AutoCmd::add_autocmd(
                AutoCmdEvent event,
                const std::string& pattern,
                const std::string& cmdstr) {
            if(event == AutoCmdEvent::UNDEFINED) {
                return ;
            }

            if(pattern.empty()) {
                return ;
            }

            auto cmd = parse_command(cmdstr) ;
            if(cmd.empty()) {
                return ;
            }

            pimpl->events_[static_cast<int>(event)].add_pattern_cmd(pattern, std::move(cmd)) ;
        }

        void AutoCmd::apply_autocmds(AutoCmdEvent event) {
            if(event == AutoCmdEvent::UNDEFINED) {
                return ;
            }

            auto evt = pimpl->events_[static_cast<int>(event)] ;

            // Get the path for executable file of the foreground window.
            auto hwnd = util::get_foreground_window() ;
            DWORD procid ;
            GetWindowThreadProcessId(hwnd, &procid) ;

            auto h_snap_raw = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) ;
            if(h_snap_raw == INVALID_HANDLE_VALUE) {
                return ;
            }
            auto close_snap = [](HANDLE handle) {CloseHandle(handle) ;} ;
            std::unique_ptr<void, decltype(close_snap)> h_snap(h_snap_raw, close_snap) ;

            PROCESSENTRY32 pe32 ;
            pe32.dwSize = sizeof(PROCESSENTRY32) ;
            if(!Process32First(h_snap.get(), &pe32)) {
                return ;
            }

            std::unordered_set<DWORD> parent_set{procid} ;
            std::unordered_set<int> pat_indices{} ;
            do {
                // Enumerate only child processs
                if(parent_set.find(pe32.th32ParentProcessID) != parent_set.end()) {
                    // Check pattern and execute sequential command
                    auto path = util::get_module_path(pe32.th32ProcessID) ;
                    auto patidx = evt.match_pattern(path) ;
                    if(patidx >= 0) {
                        pat_indices.insert(patidx) ;
                    }

                    parent_set.insert(pe32.th32ProcessID) ;
                }
            } while(Process32Next(h_snap.get(), &pe32)) ;

            if(!pat_indices.empty()) {
                for(auto patidx : pat_indices) {
                    evt.call_sequential_cmd(patidx) ;
                }
            }
        }
    }
}
