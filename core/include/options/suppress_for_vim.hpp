#ifndef _SUPPRESS_FOR_VIM_HPP
#define _SUPPRESS_FOR_VIM_HPP

#include "dynamic_option_with_creator.hpp"

class SuppressForVim : public DynamicOptionWithCreator<SuppressForVim>
{
private:
    void do_enable() const override ;
    void do_disable() const override ;
    void do_process() const override ;

public:
    static const std::string sname() noexcept ;
} ;

#endif
