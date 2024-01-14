#include "instant_mode.hpp"

#include "bind/bindedfunc.hpp"
#include "core/autocmd.hpp"
#include "core/background.hpp"
#include "core/cmdunit.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/mode.hpp"
#include "opt/blockstylecaret.hpp"
#include "opt/optionlist.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/type_traits.hpp"


namespace vind
{
    namespace bind
    {
        struct ToInstantGUINormal::Impl {
            core::Background bg_ ;

            Impl()
            : bg_(opt::search_options(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::BlockStyleCaret().name(),
                    opt::VCmdLine().name()
              ))
            {}
        } ;

        ToInstantGUINormal::ToInstantGUINormal()
        : BindedFuncFlex("to_instant_gui_normal"),
          pimpl(std::make_unique<Impl>())
        {} 
        ToInstantGUINormal::~ToInstantGUINormal() noexcept                      = default ;
        ToInstantGUINormal::ToInstantGUINormal(ToInstantGUINormal&&)            = default ;
        ToInstantGUINormal& ToInstantGUINormal::operator=(ToInstantGUINormal&&) = default ;


        SystemCall ToInstantGUINormal::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto& ihub = core::InputHub::get_instance() ;
            auto& ac = core::AutoCmd::get_instance() ;

            auto m = core::get_global_mode() ;
            ac.apply(core::get_leave_event(m)) ;
            ac.apply(core::get_enter_event(core::Mode::GUI_NORMAL)) ;

            core::InputGate::get_instance().close_all_ports_with_refresh() ;
            core::InstantKeyAbsorber isa{} ;

            opt::VCmdLine::print(opt::GeneralMessage("-- Instant GUI Normal --")) ;

            auto res = SystemCall::SUCCEEDED ;
            while(true) {
                pimpl->bg_.update() ;

                std::vector<core::CmdUnit::SPtr> inputs ;
                std::vector<std::uint16_t> in_counts ;
                if(!ihub.pull_all_inputs(
                        inputs, in_counts, core::Mode::GUI_NORMAL, true)) {
                    continue ;
                }

                while(!ihub.is_empty_queue()) {
                    core::CmdUnit::SPtr output ;
                    std::uint16_t count ;
                    if(!ihub.fetch_input(output, count)) {
                        continue ;
                    }
                    res = util::enum_or(res, output->execute(count)) ;
                }

                break ;
            }
            opt::VCmdLine::reset() ;
            ac.apply(core::get_leave_event(core::Mode::GUI_NORMAL)) ;

            return res ;
        }
    }
}
