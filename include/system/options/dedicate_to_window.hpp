#ifndef _DEDICATE_TO_WINDOW_HPP
#define _DEDICATE_TO_WINDOW_HPP

#include "dynamic_option_with_creator.hpp"

class Dedicate2Window : public DynamicOptionWithCreator<Dedicate2Window>
{
private:
    bool do_enable() const noexcept override ;
    bool do_disable() const noexcept override ;
    bool do_process() const override ;

public:
    static const std::string sname() noexcept ;

    static bool enable_targeting() noexcept ;
    static bool disable_targeting() noexcept ;
} ;

#endif
