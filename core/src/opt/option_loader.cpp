#include "opt/option_loader.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "err_logger.hpp"
#include "g_params.hpp"

#include "opt/async_uia_cache_builder.hpp"
#include "opt/autotrack_popup.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/virtual_cmd_line.hpp"


//internal linkage
namespace
{
    using namespace vind ;

    std::vector<Option::SPtr> g_vpop {
        AsyncUIACacheBuilder::create(),
        AutotrackPopup::create(),
        Dedicate2Window::create(),
        SuppressForVim::create(),
        VirtualCmdLine::create(),
    } ;
}

namespace vind
{
    namespace optloader {
        void reconstruct()
        {
            std::for_each(g_vpop.cbegin(), g_vpop.cend(), [](auto& op){op->disable() ;}) ;

            for(const auto& op : g_vpop) {
                if(gparams::get_b(op->name())) op->enable() ;
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
