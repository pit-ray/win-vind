#include "opt/autotrack_popup.hpp"

#include <iostream>
#include <windows.h>

#include "err_logger.hpp"
#include "util/def.hpp"

//internal linkage
namespace
{
    inline void set_windows_mtrack_flag(bool new_flag) {
        if(!SystemParametersInfo(SPI_SETSNAPTODEFBUTTON, new_flag, 0, SPIF_SENDCHANGE)) {
            throw RUNTIME_EXCEPT("cannot set system flag") ;
        }
    }
}

namespace vind
{
    struct AutotrackPopup::Impl {
        bool default_flag = false ;

        explicit Impl() {
            if(!SystemParametersInfo(SPI_GETSNAPTODEFBUTTON, 0,
                        reinterpret_cast<PVOID>(&default_flag), 0)) {
                throw RUNTIME_EXCEPT("cannot get system flag") ;
            }
        }

        virtual ~Impl() noexcept {
            try {
                set_windows_mtrack_flag(default_flag) ;
            }
            catch(const std::runtime_error&) {
                return ;
            }
        }
    } ;

    AutotrackPopup::AutotrackPopup()
    : pimpl(std::make_unique<Impl>())
    {}

    AutotrackPopup::~AutotrackPopup() noexcept                  = default ;
    AutotrackPopup::AutotrackPopup(AutotrackPopup&&)            = default ;
    AutotrackPopup& AutotrackPopup::operator=(AutotrackPopup&&) = default ;

    std::string AutotrackPopup::sname() noexcept {
        return "autotrack_popup" ;
    }

    void AutotrackPopup::do_enable() const {
        set_windows_mtrack_flag(true) ;
    }

    void AutotrackPopup::do_disable() const {
        set_windows_mtrack_flag(false) ;
    }

    void AutotrackPopup::do_process() const {
    }
}
