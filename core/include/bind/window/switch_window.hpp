#ifndef _SWITCH_WINDOW_HPP
#define _SWITCH_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    class SwitchWindow : public BindedFuncCreator<SwitchWindow> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit SwitchWindow() ;

        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        virtual ~SwitchWindow() noexcept ;
        SwitchWindow(SwitchWindow&&) ;
        SwitchWindow& operator=(SwitchWindow&&) ;
        SwitchWindow(const SwitchWindow&)            = delete ;
        SwitchWindow& operator=(const SwitchWindow&) = delete ;

        void load_config() override ;
    } ;
}

#endif
