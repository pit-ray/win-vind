#include "optionlist.hpp"

#include "opt/async_uia_cache_builder.hpp"
#include "opt/autotrack_popup.hpp"
#include "opt/blockstyle_caret.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/vcmdline.hpp"


namespace vind
{
    namespace opt
    {
        const std::vector<Option::SPtr>& all_global_options() {
            static const std::vector<Option::SPtr> tmp {
                AsyncUIACacheBuilder::create(),
                AutotrackPopup::create(),
                BlockStyleCaret::create(),
                Dedicate2Window::create(),
                SuppressForVim::create(),
                VCmdLine::create(),
            } ;

            return tmp ;
        }
    }
}
