#ifndef _AUTOTRACK_POPUP_HPP
#define _AUTOTRACK_POPUP_HPP

#include "dynamic_option_with_creator.hpp"

class AutotrackPopup : public DynamicOptionWithCreator<AutotrackPopup>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    bool do_enable() const noexcept override ;
    bool do_disable() const noexcept override ;
    bool do_process() const override ;

public:
    static const std::string sname() noexcept ;

    explicit AutotrackPopup() ;
    virtual ~AutotrackPopup() noexcept ;

    AutotrackPopup(AutotrackPopup&&) noexcept ;
    AutotrackPopup& operator=(AutotrackPopup&&) noexcept ;

    AutotrackPopup(const AutotrackPopup&) = delete ;
    AutotrackPopup& operator=(const AutotrackPopup&) = delete ;
} ;

#endif
