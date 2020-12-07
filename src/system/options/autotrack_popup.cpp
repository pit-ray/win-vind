#include "autotrack_popup.hpp"

#include <windows.h>
#include <iostream>

#include "msg_logger.hpp"
#include "utility.hpp"

using namespace std ;

struct AutotrackPopup::Impl
{
    bool default_flag = false ;

    explicit Impl() {
        if(!SystemParametersInfo(SPI_GETSNAPTODEFBUTTON, 0,
                    reinterpret_cast<PVOID>(&default_flag), 0)) {
            throw RUNTIME_EXCEPT("cannot get system flag") ;
        }
    }
} ;

AutotrackPopup::AutotrackPopup()
: pimpl(make_unique<Impl>())
{}

namespace APUtility
{
    inline static const auto _set_param(const bool new_flag)
    {
        if(!SystemParametersInfo(SPI_SETSNAPTODEFBUTTON, new_flag, 0, SPIF_SENDCHANGE)) {
            throw RUNTIME_EXCEPT("cannot set system flag") ;
        }
    }
}

AutotrackPopup::~AutotrackPopup() noexcept
{
    APUtility::_set_param(pimpl->default_flag) ;
}

AutotrackPopup::AutotrackPopup(AutotrackPopup&&)            = default ;
AutotrackPopup& AutotrackPopup::operator=(AutotrackPopup&&) = default ;

const string AutotrackPopup::sname() noexcept
{
    return "autotrack_popup" ;
}

void AutotrackPopup::do_enable() const
{
    APUtility::_set_param(true) ;
}

void AutotrackPopup::do_disable() const
{
    APUtility::_set_param(false) ;
}

void AutotrackPopup::do_process() const
{
}
