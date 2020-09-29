#include "autotrack_popup.hpp"
#include "msg_logger.hpp"

#include <windows.h>
#include <iostream>

using namespace std ;

struct AutotrackPopup::Impl
{
    bool default_flag = false ;

    explicit Impl() {
        if(!SystemParametersInfo(SPI_GETSNAPTODEFBUTTON, 0,
            reinterpret_cast<PVOID>(&default_flag), 0)) {

            WIN_ERROR_PRINT("cannot get system flag") ;
        }
    }
} ;

AutotrackPopup::AutotrackPopup()
: pimpl(make_unique<Impl>())
{}

namespace APUtility
{
    inline static const auto is_set_param(const bool new_flag) noexcept
    {
        if(!SystemParametersInfo(SPI_SETSNAPTODEFBUTTON, new_flag, 0, SPIF_SENDCHANGE)) {
            WIN_ERROR_PRINT("cannot set system flag") ;
            return false ;
        }
        return true ;
    }
}

AutotrackPopup::~AutotrackPopup() noexcept
{
    APUtility::is_set_param(pimpl->default_flag) ;
}

AutotrackPopup::AutotrackPopup(AutotrackPopup&&) noexcept = default ;
AutotrackPopup& AutotrackPopup::operator=(AutotrackPopup&&) noexcept = default ;

const string AutotrackPopup::sname() noexcept
{
    return "autotrack_popup" ;
}

bool AutotrackPopup::do_enable() const noexcept
{
    return APUtility::is_set_param(true) ;
}

bool AutotrackPopup::do_disable() const noexcept
{
    return APUtility::is_set_param(false) ;
}

bool AutotrackPopup::do_process() const
{
    return true ;
}
