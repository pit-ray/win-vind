#ifndef _SWITCH_WINDOW_HPP
#define _SWITCH_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        class SwitchWindow : public BindedFuncVoid<SwitchWindow> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit SwitchWindow() ;

            void sprocess() const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            virtual ~SwitchWindow() noexcept ;
            SwitchWindow(SwitchWindow&&) ;
            SwitchWindow& operator=(SwitchWindow&&) ;
            SwitchWindow(const SwitchWindow&)            = delete ;
            SwitchWindow& operator=(const SwitchWindow&) = delete ;

            void reconstruct() override ;
        } ;
    }
}

#endif
