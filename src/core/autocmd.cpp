#include "autocmd.hpp"

#include "cmdparser.hpp"
#include "cmdunit.hpp"
#include "errlogger.hpp"
#include "inputhub.hpp"
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

        void init_regex() {
            std::string regex ;
            regex.reserve(pat_.length()) ;

            for(std::size_t i = 0 ; i < pat_.length() ; i ++) {
                if(pat_[i] == '*') {
                    regex.append(".*") ;
                }
                else if(pat_[i] == '.') {
                    regex.append("\\.") ;
                }
                else {
                    regex.push_back(pat_[i]) ;
                }
            }

            re_ = std::regex(std::move(regex)) ;
        }

    public:
        explicit AutoPattern()
        : AutoPattern("")
        {}

        explicit AutoPattern(const std::string& pat)
        : pat_(pat)
        {
            init_regex() ;
        }

        explicit AutoPattern(std::string&& pat)
        : pat_(std::move(pat))
        {
            init_regex() ;
        }

        // Match a query with the pattern
        bool match(const std::string& query) {
            return std::regex_match(query, re_) ;
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
          seqcmds_()
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

        template <typename T1>
        void remove_pattern(T1&& pat) {
            auto idx = get_pat_index(pat) ;
            if(idx < 0) {
                return ;
            }

            pats_.erase(pats_.begin() + idx) ;
            seqcmds_.erase(seqcmds_.begin() + idx) ;
        }

        template <typename T1, typename T2>
        void remove_pattern_cmd(T1&& pat, T2&& cmd) {
            auto idx = get_pat_index(pat) ;
            if(idx < 0) {
                return ;
            }

            auto& cmds = seqcmds_[idx] ;

            for(std::size_t i = 0 ; i < cmds.size() ; i ++) {
                if(cmds[i].size() != cmd.size()) {
                    continue ;
                }

                bool is_same = true ;
                for(std::size_t j = 0 ; j < cmd.size() ; j ++) {
                    if(*(cmds[i][j]) != *(cmd[j])) {
                        is_same = false ;
                        break ;
                    }
                }
                if(is_same) {
                    cmds.erase(cmds.begin() + i) ;
                    return ;
                }
            }
        }

        void match_pattern(const std::string& query, std::unordered_set<int>& indices) {
            for(int i = 0 ; i < pats_.size() ; i ++) {
                if(pats_[i].match(query)) {
                    indices.insert(i) ;
                }
            }
        }

        void enqueue_sequential_command(int patidx) {
            for(const auto& cmd : seqcmds_[patidx]) {
                for(const auto& unit : cmd) {
                    if(unit->empty()) {
                        // Function
                        core::InputHub::get_instance().enqueue(unit, 1) ;
                    }
                    else {
                        // Keycode
                        core::InputHub::get_instance().do_typing(unit) ;
                    }
                }
            }
        }

        void clear() {
            pats_.clear() ;
            seqcmds_.clear() ;
        }
    } ;
}

namespace vind
{
    namespace core
    {
        AutoCmdEvent get_autocmd_event(const std::string& event_name) noexcept {
            static std::unordered_map<std::string, AutoCmdEvent> names {
                {"appenter",      AutoCmdEvent::APP_ENTER},
                {"appleave",      AutoCmdEvent::APP_LEAVE},
                {"gnormalenter",  AutoCmdEvent::GUI_NORMAL_ENTER},
                {"gnormalleave",  AutoCmdEvent::GUI_NORMAL_LEAVE},
                {"gvisualenter",  AutoCmdEvent::GUI_VISUAL_ENTER},
                {"gvisualleave",  AutoCmdEvent::GUI_VISUAL_LEAVE},
                {"enormalenter",  AutoCmdEvent::EDI_NORMAL_ENTER},
                {"enormalleave",  AutoCmdEvent::EDI_NORMAL_LEAVE},
                {"evisualenter",  AutoCmdEvent::EDI_VISUAL_ENTER},
                {"evisualleave",  AutoCmdEvent::EDI_VISUAL_LEAVE},
                {"insertenter",   AutoCmdEvent::INSERT_ENTER},
                {"insertleave",   AutoCmdEvent::INSERT_LEAVE},
                {"residententer", AutoCmdEvent::RESIDENT_ENTER},
                {"residentleave", AutoCmdEvent::RESIDENT_LEAVE},
                {"commandenter",  AutoCmdEvent::COMMAND_ENTER},
                {"commandleave",  AutoCmdEvent::COMMAND_LEAVE}
            } ;
            auto event_name_lower = util::A2a(event_name) ;
            if(names.find(event_name_lower) != names.end()) {
                return names[event_name_lower] ;
            }

            return AutoCmdEvent::UNDEFINED ;
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

        void AutoCmd::apply_autocmds(AutoCmdEvent event, DWORD procid) {
            if(event == AutoCmdEvent::UNDEFINED) {
                return ;
            }

            auto evt = pimpl->events_[static_cast<int>(event)] ;

            if(procid == 0) {
                auto hwnd = util::get_foreground_window() ;
                if(!hwnd) {
                    // There is no foreground window, so do nothing.
                    return ;
                }

                // Get the path for executable file of the foreground window.
                GetWindowThreadProcessId(hwnd, &procid) ;
            }

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

            std::unordered_set<int> pat_indices{} ;  // to call indices of the pattern list

            // Check pattern of the parent process
            std::string path ;
            try {
                path = util::A2a(util::get_module_path(procid)) ;
            }
            catch(const std::runtime_error&) {
                // If failed to get the path of the module, skip matching.
                return ;
            }
            std::unordered_set<int> indices ;
            evt.match_pattern(path, indices) ;
            if(!indices.empty()) {
                pat_indices.insert(indices.begin(), indices.end()) ;
            }

            do {
                // Check pattern of child processs
                if(parent_set.find(pe32.th32ParentProcessID) != parent_set.end()) {
                    try {
                        path = util::A2a(util::get_module_path(pe32.th32ProcessID)) ;
                    }
                    catch(const std::runtime_error&) {
                        // If failed to get the path of the module, skip matching.
                        continue ;
                    }

                    indices.clear() ;
                    evt.match_pattern(path, indices) ;
                    if(!indices.empty()) {
                        pat_indices.insert(indices.begin(), indices.end()) ;
                    }

                    parent_set.insert(pe32.th32ProcessID) ;
                }
            } while(Process32Next(h_snap.get(), &pe32)) ;

            // Execute the matched sequential commands
            if(!pat_indices.empty()) {
                for(auto patidx : pat_indices) {
                    evt.enqueue_sequential_command(patidx) ;
                }
            }
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

            auto& evt = pimpl->events_[static_cast<int>(event)] ;
            evt.add_pattern_cmd(util::A2a(pattern), std::move(cmd)) ;
        }

        void AutoCmd::remove_autocmd(AutoCmdEvent event) {
            pimpl->events_[static_cast<int>(event)].clear() ;
        }

        void AutoCmd::remove_autocmd(
            AutoCmdEvent event,
            const std::string& pattern) {
            pimpl->events_[static_cast<int>(event)].remove_pattern(pattern) ;
        }

        void AutoCmd::remove_autocmd(const std::string& pattern) {
            for(std::size_t i = 0 ; i < pimpl->events_.size() ; i ++) {
                pimpl->events_[i].remove_pattern(pattern) ;
            }
        }

        void AutoCmd::remove_autocmd(
            AutoCmdEvent event,
            const std::string& pattern,
            const std::string& cmdstr) {

            auto cmd = parse_command(cmdstr) ;
            if(cmd.empty()) {
                return ;
            }
            pimpl->events_[static_cast<int>(event)].remove_pattern_cmd(pattern, cmd) ;
        }
    }
}
