#include "optionlist.hpp"

#include "opt/autotrack_popup.hpp"
#include "opt/blockstylecaret.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"
#include "util/string.hpp"


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

        Option::SPtr ref_global_options_bynames(const std::string& name) {
            for(auto& opt : all_global_options()) {
                if(opt->name() == util::A2a(name)) {
                    return opt ;
                }
            }
            return nullptr ;
        }

        Option::SPtr ref_global_options_bynames(std::string&& name) {
            for(auto& opt : all_global_options()) {
                if(opt->name() == util::A2a(name)) {
                    return opt ;
                }
            }
            return nullptr ;
        }
    }
}
