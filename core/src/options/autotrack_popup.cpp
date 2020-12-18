#include "autotrack_popup.hpp"

#include <windows.h>
#include <iostream>

#include "msg_logger.hpp"
#include "utility.hpp"

using namespace std ;

namespace Attkpp
{
    inline static const auto _set_param(const bool new_flag)
    {
        if(!SystemParametersInfo(SPI_SETSNAPTODEFBUTTON, new_flag, 0, SPIF_SENDCHANGE)) {
            throw RUNTIME_EXCEPT("cannot set system flag") ;
        }
    }
}

struct AutotrackPopup::Impl
{
    bool default_flag = false ;

    explicit Impl() {
        if(!SystemParametersInfo(SPI_GETSNAPTODEFBUTTON, 0,
                    reinterpret_cast<PVOID>(&default_flag), 0)) {
            throw RUNTIME_EXCEPT("cannot get system flag") ;
        }
    }

    virtual ~Impl() noexcept {
        try {
            Attkpp::_set_param(default_flag) ;
        }
        catch(const std::runtime_error& e) {
            return ;
        }
    }
} ;

AutotrackPopup::AutotrackPopup()
: pimpl(make_unique<Impl>())
{}

AutotrackPopup::~AutotrackPopup() noexcept                  = default ;
AutotrackPopup::AutotrackPopup(AutotrackPopup&&)            = default ;
AutotrackPopup& AutotrackPopup::operator=(AutotrackPopup&&) = default ;

const string AutotrackPopup::sname() noexcept
{
    return "autotrack_popup" ;
}

void AutotrackPopup::do_enable() const
{
    Attkpp::_set_param(true) ;
}

void AutotrackPopup::do_disable() const
{
    Attkpp::_set_param(false) ;
}

void AutotrackPopup::do_process() const
{
}
