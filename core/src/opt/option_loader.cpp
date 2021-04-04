#include "opt/option_loader.hpp"

#include <vector>
#include <memory>
#include <algorithm>

#include "coreio/err_logger.hpp"
#include "coreio/i_params.hpp"

#include "opt/autotrack_popup.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "opt/suppress_for_vim.hpp"

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
    namespace optloader {
        void load_config()
        {
            std::for_each(g_vpop.cbegin(), g_vpop.cend(), [](auto& op){op->disable() ;}) ;

            for(const auto& op : g_vpop) {
                if(iparams::get_b(op->name())) op->enable() ;
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
