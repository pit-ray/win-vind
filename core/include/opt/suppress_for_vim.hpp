#ifndef _SUPPRESS_FOR_VIM_HPP
#define _SUPPRESS_FOR_VIM_HPP

#include "option_with_creator.hpp"

namespace vind
{
    class SuppressForVim : public OptionWithCreator<SuppressForVim>
    {
    private:
        void do_enable() const override ;
        void do_disable() const override ;
        void do_process() const override ;

    public:
        static const std::string sname() noexcept ;
    } ;
}

#endif
