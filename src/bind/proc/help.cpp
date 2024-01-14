#include "help.hpp"

#include "bind/bindinglist.hpp"
#include "core/rcparser.hpp"
#include "core/settable.hpp"
#include "opt/optionlist.hpp"
#include "util/debug.hpp"
#include "util/winwrap.hpp"

#include <stdexcept>
#include <unordered_map>


namespace
{
    using namespace vind ;

    template <typename T>
    std::string parse_query_to_url(T&& query) {
        static const std::unordered_map<std::string, std::string> query2url = {
            {"big", "/cheat_sheet/defaults/big"},
            {"cheat", "/cheat_sheet"},
            {"cheat_sheet", "/cheat_sheet"},
            {"cheatsheet", "/cheat_sheet"},
            {"default", "/cheat_sheet/defaults"},
            {"defaults", "/cheat_sheet/defaults"},
            {"down", "/downloads"},
            {"download", "/downloads"},
            {"downloads", "/downloads"},
            {"faq", "/faq"},
            {"fn", "/cheat_sheet/functions"},
            {"func", "/cheat_sheet/functions"},
            {"functions", "/cheat_sheet/functions"},
            {"home", ""},
            {"huge", "/cheat_sheet/defaults/huge"},
            {"issue", "/faq"},
            {"issues", "/faq"},
            {"keyword", "/cheat_sheet/keywords"},
            {"keywords", "/cheat_sheet/keywords"},
            {"manual", "/usage"},
            {"mode", "/cheat_sheet/keywords/#mode-prefix"},
            {"normal", "/cheat_sheet/defaults/normal"},
            {"opt", "/cheat_sheet/options"},
            {"option", "/cheat_sheet/options"},
            {"options", "/cheat_sheet/options"},
            {"parameters", "/cheat_sheet/options"},
            {"params", "/cheat_sheet/options"},
            {"small", "/cheat_sheet/defaults/small"},
            {"tier", "/cheat_sheet/defaults"},
            {"tiers", "/cheat_sheet/defaults"},
            {"tiny", "/cheat_sheet/defaults/tiny"},
            {"usage", "/usage"},
            {"version", "/cheat_sheet/defaults"},
        } ;
        return query2url.at(util::A2a(query)) ;
    }
}


namespace vind
{
    namespace bind
    {
        Help::Help()
        : BindedFuncVoid("help")
        {}

        void Help::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args) {
            static constexpr auto base_url = "https://pit-ray.github.io/win-vind" ;

            auto [_, pargs] = core::divide_cmd_and_args(args) ;
            if(pargs.empty()) {
                util::shell_execute(base_url) ;
                return ;
            }

            // Function names
            if(check_if_func(pargs)) {
                auto sub_url = "/cheat_sheet/functions/#" + pargs ;
                util::shell_execute(base_url + sub_url) ;
                return ;
            }

            // Option names
            if(opt::check_if_option(pargs)) {
                auto sub_url = "/cheat_sheet/options/#" + pargs ;
                util::shell_execute(base_url + sub_url) ;
                return ;
            }

            // Parameter names
            if(core::SetTable::get_instance().has(pargs)) {
                auto sub_url = "/cheat_sheet/options/#" + pargs ;
                util::shell_execute(base_url + sub_url) ;
                return ;
            }

            // Predefined queries
            try {
                auto sub_url = parse_query_to_url(pargs) ;
                util::shell_execute(base_url + sub_url) ;
            }
            catch(const std::out_of_range&) {
                // If the argument is invalid, open the top page.
                util::shell_execute(base_url) ;
            }
        }
    }
}
