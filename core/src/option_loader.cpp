#include "option_loader.hpp"

#include <vector>
#include <memory>
#include <algorithm>

#include "msg_logger.hpp"
#include "i_params.hpp"

#include "autotrack_popup.hpp"
#include "dedicate_to_window.hpp"
#include "virtual_cmd_line.hpp"
#include "suppress_for_vim.hpp"

//internal linkage
namespace
{
    using namespace vind ;

    std::vector<op::shp_t> g_vpop {
        AutotrackPopup::create(),
        VirtualCmdLine::create(),
        Dedicate2Window::create(),
        SuppressForVim::create()
    } ;
}

namespace vind
{
    namespace OptionLoader {
        void load_config()
        {
            std::for_each(g_vpop.cbegin(), g_vpop.cend(), [](auto& op){op->disable() ;}) ;

            for(const auto& op : g_vpop) {
                if(iParams::get_b(op->name())) op->enable() ;
            }
        }

        void call_active_funcs()
        {
            for(const auto& op : g_vpop) {
                op->process() ;
            }
        }
    }
}
