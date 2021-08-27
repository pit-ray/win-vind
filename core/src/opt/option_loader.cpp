#include "opt/option_loader.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "err_logger.hpp"
#include "g_params.hpp"

#include "opt/async_uia_cache_builder.hpp"
#include "opt/autotrack_popup.hpp"
#include "opt/blockstyle_caret.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/vcmdline.hpp"


//internal linkage
namespace
{
    using namespace vind ;

    std::vector<Option::SPtr> g_vpop {
        AsyncUIACacheBuilder::create(),
        AutotrackPopup::create(),
        BlockStyleCaret::create(),
        Dedicate2Window::create(),
        SuppressForVim::create(),
        VCmdLine::create(),
    } ;
}

namespace vind
{
    namespace optloader {
        void reconstruct()
        {
            for(const auto& op : g_vpop) {
                if(gparams::get_b(op->name())) {
                    op->enable() ;
                }
                else {
                    op->disable() ;
                }
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
