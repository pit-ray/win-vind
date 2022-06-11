#ifndef _WINDOW_RESIZER_HPP
#define _WINDOW_RESIZER_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        class WindowResizer : public BindedFuncVoid<WindowResizer> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit WindowResizer() ;

            void sprocess() ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            virtual ~WindowResizer() noexcept ;
            WindowResizer(WindowResizer&&) ;
            WindowResizer& operator=(WindowResizer&&) ;
            WindowResizer(const WindowResizer&)            = delete ;
            WindowResizer& operator=(const WindowResizer&) = delete ;

            void reconstruct() override ;
        } ;
    }
}

#endif
