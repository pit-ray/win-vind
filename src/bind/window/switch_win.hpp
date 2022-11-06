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

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            virtual ~SwitchWindow() noexcept ;
            SwitchWindow(SwitchWindow&&) ;
            SwitchWindow& operator=(SwitchWindow&&) ;
            SwitchWindow(const SwitchWindow&)            = delete ;
            SwitchWindow& operator=(const SwitchWindow&) = delete ;
        } ;
    }
}

#endif
