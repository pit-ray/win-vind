#ifndef _AUTOTRACK_POPUP_HPP
#define _AUTOTRACK_POPUP_HPP

#include "option.hpp"

namespace vind
{
    namespace opt
    {
        class AutotrackPopup : public OptionCreator<AutotrackPopup> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            void do_enable() override ;
            void do_disable() override ;
            void do_process() override ;

        public:
            explicit AutotrackPopup() ;
            virtual ~AutotrackPopup() noexcept ;

            AutotrackPopup(AutotrackPopup&&) ;
            AutotrackPopup& operator=(AutotrackPopup&&)  ;

            AutotrackPopup(const AutotrackPopup&)            = delete ;
            AutotrackPopup& operator=(const AutotrackPopup&) = delete ;
        } ;
    }
}

#endif
