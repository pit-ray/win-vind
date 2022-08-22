#include "d2wctrl.hpp"

#include "opt/dedicate_to_window.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //Enable
        EnableTargetingOfDedicate2Window::EnableTargetingOfDedicate2Window()
        : BindedFuncVoid("enable_targeting_of_dedicate_to_window")
        {}
        void EnableTargetingOfDedicate2Window::sprocess(
                std::uint16_t UNUSED(count), const std::string& UNUSED(args)) {
            opt::Dedicate2Window::enable_targeting() ;
        }

        //Disable
        DisableTargetingOfDedicate2Window::DisableTargetingOfDedicate2Window()
        : BindedFuncVoid("disable_targeting_of_dedicate_to_window")
        {}
        void DisableTargetingOfDedicate2Window::sprocess(
                std::uint16_t UNUSED(count), const std::string& UNUSED(args)) {
            opt::Dedicate2Window::disable_targeting() ;
        }
    }
}
