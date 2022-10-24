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

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            virtual ~WindowResizer() noexcept ;
            WindowResizer(WindowResizer&&) ;
            WindowResizer& operator=(WindowResizer&&) ;
            WindowResizer(const WindowResizer&)            = delete ;
            WindowResizer& operator=(const WindowResizer&) = delete ;
        } ;
    }
}

#endif
