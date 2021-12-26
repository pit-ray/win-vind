#include "instant_mode.hpp"

#include "bind/binded_func.hpp"
#include "core/background.hpp"
#include "core/entry.hpp"
#include "core/func_finder.hpp"
#include "core/inputgate.hpp"
#include "core/key_logger_base.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "opt/async_uia_cache_builder.hpp"
#include "opt/blockstyle_caret.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/optionlist.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        struct ToInstantGUINormal::Impl {
            core::FuncFinder finder_ ;
            core::Background bg_ ;

            Impl()
            : finder_(),
              bg_(opt::ref_global_options_bynames(
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


        SystemCall ToInstantGUINormal::sprocess() const {
            auto& igate = core::InputGate::get_instance() ;
            igate.close_all_ports_with_refresh() ;

            core::InstantKeyAbsorber isa{} ;

            opt::VCmdLine::print(opt::GeneralMessage("-- Instant GUI Normal --")) ;

            constexpr auto lcx_vmode = core::Mode::GUI_NORMAL ;

            pimpl->finder_.reset_parser_states(lcx_vmode) ;
            core::NTypeLogger lgr ;

            auto syscal = SystemCall::NOTHING ;

            while(true) {
                pimpl->bg_.update() ;

                auto log = igate.pop_log() ;
                auto result = lgr.logging_state(log) ;
                if(NTYPE_EMPTY(result)) {
                    continue ;
                }
                if(NTYPE_HEAD_NUM(result)) {
                    lgr.reject() ;
                    continue ;
                }

                if(auto parser = pimpl->finder_.find_parser_with_transition(
                            lgr.latest(), 0, lcx_vmode)) {

                    if(parser->is_accepted()) {
                        pimpl->finder_.reset_parser_states(lcx_vmode) ;
                        syscal = parser->get_func()->process(lgr) ;
                        break ;
                    }
                    else if(parser->is_rejected_with_ready()) {
                        // It did not accepted, but only matched subsets.
                        // For example, bindings <ctrl> in <ctrl-f>
                        pimpl->finder_.backward_parser_states(1, lcx_vmode) ;
                        lgr.remove_from_back(1) ;
                    }
                }
                else {
                    break ;
                }
            }
            opt::VCmdLine::reset() ;

            return syscal ;
        }

        SystemCall ToInstantGUINormal::sprocess(core::NTypeLogger& parent) const {
            if(!parent.is_long_pressing()) {
                return sprocess() ;
            }
            return SystemCall::NOTHING ;
        }

        SystemCall ToInstantGUINormal::sprocess(const core::CharLogger& UNUSED(parent)) const {
            return sprocess() ;
        }

        void ToInstantGUINormal::reconstruct() {
            pimpl->finder_.reconstruct() ;
        }
    }
}
