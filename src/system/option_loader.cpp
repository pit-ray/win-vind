#include "option_loader.hpp"

#include <vector>
#include <memory>
#include <algorithm>

#include "msg_logger.hpp"
#include "i_params.hpp"

#include "autotrack_popup.hpp"
#include "dedicate_to_window.hpp"
#include "virtual_cmd_line.hpp"


namespace OptionLoader
{
    using namespace std ;
    vector<op::shp_t> vpop {
        AutotrackPopup::create(),
        VirtualCmdLine::create(),
        Dedicate2Window::create()
    } ;

    void load_config() noexcept
    {
        //load options ------------------------------------------------
        //initialzie
        for_each(vpop.cbegin(), vpop.cend(), [](auto& op){op->disable() ;}) ;

        for(const auto& op : vpop) {
            if(iParams::get_b(op->name())) op->enable() ;
        }
    }

    void call_active_funcs() noexcept
    {
        for(const auto& op : vpop) {
            op->process() ;
        }
    }
}
