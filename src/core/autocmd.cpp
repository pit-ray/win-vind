#include "autocmd.hpp"
#include "bind/bindedfunc.hpp"

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>


namespace
{
    using namespace vind ;

    struct SequentialCmd {
        int a = 0 ;
    } ;

    class AutoEvent {
    private:
        std::unordered_map<std::string, SequentialCmd> pats_ ;

    public:
        explicit AutoEvent()
        : pats_()
        {}

        void add_pattern_cmd(const std::string& pat, const SequentialCmd& cmd) {
        }

        bool has_pattern(const std::string& pat) {
            return true ;
        }

        const SequentialCmd& get_sequential_cmd(const std::string& pat) {
            return pats_.at(pat) ;
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
                {"CmdLineEnter",    AutoCmdEvent::CMDLINE_ENTER},
                {"CmdLineLeave",    AutoCmdEvent::CMDLINE_LEAVE}
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
            const std::string& cmd) {

        }

        void AutoCmd::apply_autocmds(AutoCmdEvent event) {
            auto evt = pimpl->events_[static_cast<int>(event)] ;

            // Get the path for executable file of the foreground window.

            // Check pattern

            // Do sequential command

        }
    }
}
