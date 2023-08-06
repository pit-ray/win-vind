#ifndef _AUTOCMD_HPP
#define _AUTOCMD_HPP

#include "mode.hpp"

#include <windows.h>

#include <memory>
#include <string>
#include <unordered_map>


namespace vind
{
    namespace core
    {
        enum class AutoCmdEvent : unsigned char {
            PROC_ENTER,
            PROC_LEAVE,
            GUI_NORMAL_ENTER,
            GUI_NORMAL_LEAVE,
            GUI_VISUAL_ENTER,
            GUI_VISUAL_LEAVE,
            EDI_NORMAL_ENTER,
            EDI_NORMAL_LEAVE,
            EDI_VISUAL_ENTER,
            EDI_VISUAL_LEAVE,
            INSERT_ENTER,
            INSERT_LEAVE,
            RESIDENT_ENTER,
            RESIDENT_LEAVE,
            COMMAND_ENTER,
            COMMAND_LEAVE,

            EVENT_NUM,
            UNDEFINED,
        } ;

        AutoCmdEvent get_autocmd_event(const std::string& event_name) noexcept ;

        inline AutoCmdEvent get_leave_event(Mode mode) noexcept {
            static const std::unordered_map<Mode, AutoCmdEvent> leave_events {
                {Mode::GUI_NORMAL, AutoCmdEvent::GUI_NORMAL_LEAVE},
                {Mode::GUI_VISUAL, AutoCmdEvent::GUI_VISUAL_LEAVE},
                {Mode::EDI_NORMAL, AutoCmdEvent::EDI_NORMAL_LEAVE},
                {Mode::EDI_VISUAL, AutoCmdEvent::EDI_VISUAL_LEAVE},
                {Mode::INSERT,     AutoCmdEvent::INSERT_LEAVE},
                {Mode::RESIDENT,   AutoCmdEvent::RESIDENT_LEAVE},
                {Mode::COMMAND,    AutoCmdEvent::COMMAND_LEAVE}
            } ;
            if(leave_events.find(mode) != leave_events.end()) {
                return leave_events.at(mode) ;
            }
            return AutoCmdEvent::UNDEFINED ;
        }

        inline AutoCmdEvent get_enter_event(Mode mode) noexcept {
            static const std::unordered_map<Mode, AutoCmdEvent> enter_events {
                {Mode::GUI_NORMAL, AutoCmdEvent::GUI_NORMAL_ENTER},
                {Mode::GUI_VISUAL, AutoCmdEvent::GUI_VISUAL_ENTER},
                {Mode::EDI_NORMAL, AutoCmdEvent::EDI_NORMAL_ENTER},
                {Mode::EDI_VISUAL, AutoCmdEvent::EDI_VISUAL_ENTER},
                {Mode::INSERT,     AutoCmdEvent::INSERT_ENTER},
                {Mode::RESIDENT,   AutoCmdEvent::RESIDENT_ENTER},
                {Mode::COMMAND,    AutoCmdEvent::COMMAND_ENTER}
            } ;
            if(enter_events.find(mode) != enter_events.end()) {
                return enter_events.at(mode) ;
            }
            return AutoCmdEvent::UNDEFINED ;
        }

        class AutoCmd {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            explicit AutoCmd() ;
            virtual ~AutoCmd() noexcept ;

        public:
            static AutoCmd& get_instance() ;

            // Perform command automation.
            // First argument: the event type
            // Second argument: target window handle, which has default HWND.
            void apply_autocmds(AutoCmdEvent event, DWORD procid=0) ;

            void add_autocmd(
                AutoCmdEvent event,
                const std::string& pattern,
                const std::string& cmd) ;

            void remove_autocmd(AutoCmdEvent event) ;

            void remove_autocmd(
                AutoCmdEvent event,
                const std::string& pattern) ;

            void remove_autocmd(const std::string& pattern) ;

            void remove_autocmd(
                AutoCmdEvent event,
                const std::string& pattern,
                const std::string& cmd) ;
        } ;
    }
}

#endif
