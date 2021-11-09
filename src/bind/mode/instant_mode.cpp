#include "instant_mode.hpp"

#include "bind/binded_func.hpp"
#include "bind/func_finder.hpp"
#include "core/entry.hpp"
#include "core/key_absorber.hpp"
#include "core/key_logger_base.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    struct ToInstantGUINormal::Impl {
        FuncFinder finder_{} ;
    } ;

    ToInstantGUINormal::ToInstantGUINormal()
    : BindedFuncCreator("to_instant_gui_normal"),
      pimpl(std::make_unique<Impl>())
    {} 
    ToInstantGUINormal::~ToInstantGUINormal() noexcept                      = default ;
    ToInstantGUINormal::ToInstantGUINormal(ToInstantGUINormal&&)            = default ;
    ToInstantGUINormal& ToInstantGUINormal::operator=(ToInstantGUINormal&&) = default ;


    void ToInstantGUINormal::sprocess() const {
        keyabsorber::close_all_ports_with_refresh() ;

        keyabsorber::InstantKeyAbsorber isa{} ;

        opt::VCmdLine::print(opt::GeneralMessage("-- Instant GUI Normal --")) ;

        constexpr auto lcx_vmode = mode::Mode::GUI_NORMAL ;

        pimpl->finder_.reset_parser_states(lcx_vmode) ;
        NTypeLogger lgr ;
        while(vind::update_background()) {
            auto result = lgr.logging_state() ;
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
                    parser->get_func()->process(lgr) ;
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
    }

    void ToInstantGUINormal::sprocess(NTypeLogger& parent) const {
        if(!parent.is_long_pressing()) {
            sprocess() ;
        }
    }

    void ToInstantGUINormal::sprocess(const CharLogger& UNUSED(parent)) const {
        sprocess() ;
    }

    void ToInstantGUINormal::reconstruct() {
        pimpl->finder_.reconstruct_funcset() ;
    }
}
