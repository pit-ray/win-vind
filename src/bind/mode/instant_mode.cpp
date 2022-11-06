#include "instant_mode.hpp"

#include "bind/bindedfunc.hpp"
#include "core/background.hpp"
#include "core/cmdunit.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/mode.hpp"
#include "opt/blockstylecaret.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/optionlist.hpp"
#include "opt/suppress_for_vim.hpp"
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
            : bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::BlockStyleCaret().name(),
                    opt::Dedicate2Window().name(),
                    opt::SuppressForVim().name(),
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
            core::InputGate::get_instance().close_all_ports_with_refresh() ;
            core::InstantKeyAbsorber isa{} ;

            opt::VCmdLine::print(opt::GeneralMessage("-- Instant GUI Normal --")) ;

            auto syscal = SystemCall::SUCCEEDED ;
            while(true) {
                pimpl->bg_.update() ;

                core::CmdUnit::SPtr input ;
                std::vector<core::CmdUnit::SPtr> outputs ;
                std::vector<std::uint16_t> counts ;
                core::InputHub::get_instance().fetch_inputs(
                        input, outputs, counts, core::Mode::GUI_NORMAL) ;

                if(!input || outputs.empty()) {
                    continue ;
                }

                auto res = SystemCall::SUCCEEDED ;
                for(std::size_t i = 0 ; i < outputs.size() ; i ++) {
                    res = util::enum_or(res, outputs[i]->execute(counts[i])) ;
                }

                break ;
            }
            opt::VCmdLine::reset() ;

            return syscal ;
        }
    }
}
