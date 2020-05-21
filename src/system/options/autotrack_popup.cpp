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

            ERROR_STREAM << "windows.h " << GetLastError() << " (AutotrackPopup.cpp)\n" ;
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
            ERROR_STREAM << " windows.h " << GetLastError() << " (AutotrackPopup.cpp)\n" ;
            return false ;
        }
        return true ;
    }
}

AutotrackPopup::~AutotrackPopup()
{
    APUtility::is_set_param(pimpl->default_flag) ;
    MESSAGE_STREAM << "Refreshed successfully (Option: " << name() << ")\n" ;
}

AutotrackPopup::AutotrackPopup(AutotrackPopup&&) = default ;
AutotrackPopup& AutotrackPopup::operator=(AutotrackPopup&&) = default ;

const string AutotrackPopup::name() const noexcept
{
    return "autotrack_popup" ;
}

unique_ptr<DynamicOption> AutotrackPopup::create()
{
    return std::move(make_unique<AutotrackPopup>()) ;
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
