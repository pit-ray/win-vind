#ifndef _DEDICATE_TO_WINDOW_HPP
#define _DEDICATE_TO_WINDOW_HPP

#include "dynamic_option_with_creator.hpp"

class Dedicate2Window : public DynamicOptionWithCreator<Dedicate2Window>
{
private:
    void do_enable() const override ;
    void do_disable() const override ;
    void do_process() const override ;

public:
    static const std::string sname() noexcept ;

    static void enable_targeting() ;
    static void disable_targeting() ;
} ;

#endif
