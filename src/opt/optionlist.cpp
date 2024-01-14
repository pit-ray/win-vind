#include "optionlist.hpp"

#include "opt/autotrack_popup.hpp"
#include "opt/blockstylecaret.hpp"
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
                VCmdLine::create(),
            } ;

            return tmp ;
        }

        Option::SPtr search_option(const std::string& name) {
            for(auto& opt : all_global_options()) {
                if(opt->name() == util::A2a(name)) {
                    return opt ;
                }
            }
            return nullptr ;
        }

        Option::SPtr search_option(std::string&& name) {
            for(auto& opt : all_global_options()) {
                if(opt->name() == util::A2a(std::move(name))) {
                    return opt ;
                }
            }
            return nullptr ;
        }
    }
}
