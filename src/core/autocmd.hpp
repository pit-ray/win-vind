#ifndef _AUTOCMD_HPP
#define _AUTOCMD_HPP

#include <memory>
#include <string>


namespace vind
{
    namespace core
    {
        enum class AutoCmdEvent : unsigned char {
            WIN_ENTER,
            WIN_LEAVE,
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
            CMDLINE_ENTER,
            CMDLINE_LEAVE,

            EVENT_NUM
        } ;


        AutoCmdEvent get_autocmd_event(const std::string& event_name) ;

        class AutoCmd {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            explicit AutoCmd() ;
            virtual ~AutoCmd() noexcept ;

        public:
            static AutoCmd& get_instance() ;

            void add_autocmd(
                AutoCmdEvent event,
                const std::string& pattern,
                const std::string& cmd) ;

            void apply_autocmds(AutoCmdEvent event) ;
        } ;
    }
}

#endif
