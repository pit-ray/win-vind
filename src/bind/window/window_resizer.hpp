#ifndef _WINDOW_RESIZER_HPP
#define _WINDOW_RESIZER_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    class WindowResizer : public BindedFuncCreator<WindowResizer> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit WindowResizer() ;

        void sprocess() const ;
        void sprocess(core::NTypeLogger& parent_lgr) const ;
        void sprocess(const core::CharLogger& parent_lgr) const ;

        virtual ~WindowResizer() noexcept ;
        WindowResizer(WindowResizer&&) ;
        WindowResizer& operator=(WindowResizer&&) ;
        WindowResizer(const WindowResizer&)            = delete ;
        WindowResizer& operator=(const WindowResizer&) = delete ;

        void reconstruct() override ;
    } ;
}

#endif
