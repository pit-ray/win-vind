#include "opt/option_loader.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "err_logger.hpp"
#include "g_params.hpp"

#include "opt/autotrack_popup.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/vcmdline.hpp"

//internal linkage
namespace
{
    using namespace vind ;

    std::vector<op::SPtr> g_vpop {
        AutotrackPopup::create(),
        VirtualCmdLine::create(),
        Dedicate2Window::create(),
        SuppressForVim::create()
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
