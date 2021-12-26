#ifndef _SUPPRESS_FOR_VIM_HPP
#define _SUPPRESS_FOR_VIM_HPP

#include "option.hpp"

namespace vind
{
    namespace opt
    {
        class SuppressForVim : public OptionCreator<SuppressForVim> {
        private:
            void do_enable() const override ;
            void do_disable() const override ;
            void do_process() const override ;

        public:
            explicit SuppressForVim() ;
        } ;
    }
}

#endif
