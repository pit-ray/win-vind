#ifndef _AUTOTRACK_POPUP_HPP
#define _AUTOTRACK_POPUP_HPP

#include "dynamic_option.hpp"

struct AutotrackPopup : public DynamicOption
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    bool do_enable() const noexcept override ;
    bool do_disable() const noexcept override ;
    bool do_process() const override ;

public:
    const std::string name() const noexcept override ;
    static std::unique_ptr<DynamicOption> create() ;

    explicit AutotrackPopup() ;
    ~AutotrackPopup() ;

    AutotrackPopup(AutotrackPopup&&) ;
    AutotrackPopup& operator=(AutotrackPopup&&) ;

    AutotrackPopup(const AutotrackPopup&) = delete ;
    AutotrackPopup& operator=(const AutotrackPopup&) = delete ;
} ;


#endif