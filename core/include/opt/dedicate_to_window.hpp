#ifndef _DEDICATE_TO_WINDOW_HPP
#define _DEDICATE_TO_WINDOW_HPP

#include "option_creator.hpp"

namespace vind
{
    class Dedicate2Window : public OptionCreator<Dedicate2Window>
    {
    private:
        void do_enable() const override ;
        void do_disable() const override ;
        void do_process() const override ;

    public:
        explicit Dedicate2Window() ;
        static void enable_targeting() ;
        static void disable_targeting() ;
    } ;
}
#endif
