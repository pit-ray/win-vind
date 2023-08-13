#include "autocmd.hpp"

#include "cmdparser.hpp"
#include "cmdunit.hpp"
#include "errlogger.hpp"
#include "inputhub.hpp"
#include "util/winwrap.hpp"

#include <array>
#include <chrono>
#include <memory>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <tlhelp32.h>


namespace
{
    using namespace vind ;

    class AutoPattern {
    private:
        std::string pat_ ;
        std::regex re_ ;
        bool is_absolute ;

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
                else if(pat_[i] == '?' && (i == 0 || (i > 0 && pat_[i - 1] != '\\'))) {
                    regex.append(".") ;
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
        : pat_(pat),
          re_(),
          is_absolute(pat_.find('/') != std::string::npos)
        {
            init_regex() ;
        }

        explicit AutoPattern(std::string&& pat)
        : pat_(std::move(pat)),
          re_(),
          is_absolute(pat_.find('/') != std::string::npos)
        {
            init_regex() ;
        }

        // Match a query with the pattern
        bool match(const std::filesystem::path& query) {
            std::string str_query ;
            if(is_absolute) {
                str_query = query.u8string() ;
            }
            else {
                str_query = query.filename().u8string() ;
            }

            return std::regex_match(str_query, re_) ;
        }

        bool match(std::string&& query) {
            return match(std::filesystem::path(std::move(query))) ;
        }

        bool match(const std::string& query) {
            return match(std::filesystem::path(query)) ;
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
            for(std::size_t i = 0 ; i < pats_.size() ; i ++) {
                if(pats_[i].get() == query) {
                    return static_cast<int>(i) ;
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

        void match_pattern(
                const std::filesystem::path& query,
                std::unordered_set<int>& indices) {
            for(std::size_t i = 0 ; i < pats_.size() ; i ++) {
                if(pats_[i].match(query)) {
                    indices.insert(static_cast<int>(i)) ;
                }
            }
        }

        const SequentialCmd& get_sequential_command(int patidx) {
            return seqcmds_.at(patidx) ;
        }

        void clear() {
            pats_.clear() ;
            seqcmds_.clear() ;
        }
    } ;

    static auto close_snap = [](HANDLE handle) {CloseHandle(handle) ;} ;
}

namespace vind
{
    namespace core
    {
        AutoCmdEvent get_autocmd_event(const std::string& event_name) {
            static std::unordered_map<std::string, AutoCmdEvent> names {
                {"appenter",      AutoCmdEvent::APP_ENTER},
                {"appleave",      AutoCmdEvent::APP_LEAVE},
                {"guinormalenter",  AutoCmdEvent::GUI_NORMAL_ENTER},
                {"guinormalleave",  AutoCmdEvent::GUI_NORMAL_LEAVE},
                {"guivisualenter",  AutoCmdEvent::GUI_VISUAL_ENTER},
                {"guivisualleave",  AutoCmdEvent::GUI_VISUAL_LEAVE},
                {"edinormalenter",  AutoCmdEvent::EDI_NORMAL_ENTER},
                {"edinormalleave",  AutoCmdEvent::EDI_NORMAL_LEAVE},
                {"edivisualenter",  AutoCmdEvent::EDI_VISUAL_ENTER},
                {"edivisualleave",  AutoCmdEvent::EDI_VISUAL_LEAVE},
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

        auto close_snap = [](HANDLE handle) {CloseHandle(handle) ;} ;
        struct AutoCmd::Impl {
            std::array<AutoEvent, static_cast<int>(AutoCmdEvent::EVENT_NUM)> events_ ;

            std::unique_ptr<void, decltype(close_snap)> h_snap ;
            std::chrono::system_clock::time_point snap_timestamp ;

            explicit Impl()
            : events_(),
              h_snap(nullptr, close_snap),
              snap_timestamp()
            {}

            std::filesystem::path get_module_path(DWORD procid) {
                // Check pattern of the parent process
                std::filesystem::path path ;
                try {
                    path = util::get_module_path(procid) ;
                }
                catch(const std::runtime_error&) {
                    return std::filesystem::path() ;
                }
                return std::filesystem::path(
                    util::A2a(util::replace_all(path.u8string(), "\\", "/"))) ;
            }
        } ;

        AutoCmd::AutoCmd()
        : pimpl(std::make_unique<Impl>())
        {}

        AutoCmd::~AutoCmd() noexcept = default ;

        AutoCmd& AutoCmd::get_instance() {
            static AutoCmd instance{} ;
            return instance ;
        }

        void AutoCmd::apply(AutoCmdEvent event, DWORD procid) {
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

            using namespace std::chrono ;
            auto cache_lifetime = duration_cast<seconds>(
                    system_clock::now() - pimpl->snap_timestamp) ;

            // Avoid tasking snapshots too frequently
            if(!pimpl->h_snap || cache_lifetime > 1s) {
                auto h_snap_raw = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) ;
                if(h_snap_raw == INVALID_HANDLE_VALUE) {
                    return ;
                }
                pimpl->h_snap.reset(h_snap_raw) ;
                pimpl->snap_timestamp = system_clock::now() ;
            }

            PROCESSENTRY32 pe32 ;
            pe32.dwSize = sizeof(PROCESSENTRY32) ;
            if(!Process32First(pimpl->h_snap.get(), &pe32)) {
                return ;
            }

            std::unordered_set<DWORD> parent_set{procid} ;
            std::unordered_set<int> pat_indices{} ;  // to call indices of the pattern list

            auto path = pimpl->get_module_path(procid) ;
            if(path.empty()) {
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
                    path = pimpl->get_module_path(pe32.th32ProcessID) ;
                    if(path.empty()) {
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
            } while(Process32Next(pimpl->h_snap.get(), &pe32)) ;

            // Execute the matched sequential commands
            if(!pat_indices.empty()) {
                for(auto patidx : pat_indices) {
                    for(const auto& cmd : evt.get_sequential_command(patidx)) {
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
            }
        }

        void AutoCmd::add(
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

        void AutoCmd::remove(AutoCmdEvent event) {
            pimpl->events_[static_cast<int>(event)].clear() ;
        }

        void AutoCmd::remove(
                AutoCmdEvent event,
                const std::string& pattern) {
            pimpl->events_[static_cast<int>(event)].remove_pattern(pattern) ;
        }

        void AutoCmd::remove(const std::string& pattern) {
            for(std::size_t i = 0 ; i < pimpl->events_.size() ; i ++) {
                pimpl->events_[i].remove_pattern(pattern) ;
            }
        }
    }
}
