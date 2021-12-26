#ifndef _DEDICATE_TO_WINDOW_HPP
#define _DEDICATE_TO_WINDOW_HPP

#include "option.hpp"

#include <windows.h>


namespace vind
{
    namespace opt
    {
        class Dedicate2Window : public OptionCreator<Dedicate2Window> {
        private:
            void do_enable() const override ;
            void do_disable() const override ;
            void do_process() const override ;

            static HWND target_hwnd_ ;
            static HWND past_hwnd_ ;

        public:
            explicit Dedicate2Window() ;
            static void enable_targeting() ;
            static void disable_targeting() ;
        } ;
    }
}
#endif
