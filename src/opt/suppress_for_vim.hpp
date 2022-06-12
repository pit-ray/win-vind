#ifndef _SUPPRESS_FOR_VIM_HPP
#define _SUPPRESS_FOR_VIM_HPP

#include "option.hpp"

namespace vind
{
    namespace opt
    {
        class SuppressForVim : public OptionCreator<SuppressForVim> {
        private:
            void do_enable() override ;
            void do_disable() override ;
            void do_process() override ;

        public:
            explicit SuppressForVim() ;
        } ;
    }
}

#endif
