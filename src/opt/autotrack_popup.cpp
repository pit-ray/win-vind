#include "opt/autotrack_popup.hpp"

#include <windows.h>

#include "core/errlogger.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"


namespace
{
    inline auto get_property() {
        BOOL val ;
        vind::util::system_parameters_info(
                SPI_GETSNAPTODEFBUTTON,
                0, &val, 0) ;
        return val ;
    }

    inline auto set_property(BOOL val) {
        vind::util::system_parameters_info(
                SPI_SETSNAPTODEFBUTTON,
                val, 0,
                SPIF_UPDATEINIFILE | SPIF_SENDCHANGE) ;
    }
}


namespace vind
{
    namespace opt
    {
        struct AutotrackPopup::Impl {
            BOOL sysflag_ = FALSE ;
        } ;

        AutotrackPopup::AutotrackPopup()
        : OptionCreator("autotrack_popup"),
          pimpl(std::make_unique<Impl>())
        {
            pimpl->sysflag_ = get_property() ;
        }

        AutotrackPopup::~AutotrackPopup() noexcept {
            try {
                if(get_property() != pimpl->sysflag_) {
                    set_property(pimpl->sysflag_) ;
                }
            }
            catch(const std::runtime_error& e) {
                PRINT_ERROR(e.what()) ;
            }
        }
        AutotrackPopup::AutotrackPopup(AutotrackPopup&&)            = default ;
        AutotrackPopup& AutotrackPopup::operator=(AutotrackPopup&&) = default ;

        void AutotrackPopup::do_enable() {
            if(get_property() != TRUE) {
                set_property(TRUE) ;
            }
        }

        void AutotrackPopup::do_disable() {
            if(get_property() != FALSE) {
                set_property(FALSE) ;
            }
        }

        void AutotrackPopup::do_process() {
        }
    }
}
