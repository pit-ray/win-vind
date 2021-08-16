#ifndef _AUTOTRACK_POPUP_HPP
#define _AUTOTRACK_POPUP_HPP

#include "option_creator.hpp"

namespace vind
{
    class AutotrackPopup : public OptionCreator<AutotrackPopup>
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void do_enable() const override ;
        void do_disable() const override ;
        void do_process() const override ;

    public:
        explicit AutotrackPopup() ;
        virtual ~AutotrackPopup() noexcept ;

        AutotrackPopup(AutotrackPopup&&) ;
        AutotrackPopup& operator=(AutotrackPopup&&)  ;

        AutotrackPopup(const AutotrackPopup&)            = delete ;
        AutotrackPopup& operator=(const AutotrackPopup&) = delete ;
    } ;
}

#endif
