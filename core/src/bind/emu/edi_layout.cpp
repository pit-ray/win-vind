#include "bind/emu/edi_layout.hpp"

#include "bind/base/mode.hpp"
#include "bind/base/ntype_logger.hpp"
#include "bind/base/safe_repeater.hpp"
#include "io/keybrd.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"


namespace vind
{
    struct EdiNRemoveEOL::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNRemoveEOL::EdiNRemoveEOL()
    : BindedFuncCreator("edi_n_remove_EOL"),
      pimpl(std::make_unique<Impl>())
    {}

    EdiNRemoveEOL::~EdiNRemoveEOL() noexcept                    = default ;
    EdiNRemoveEOL::EdiNRemoveEOL(EdiNRemoveEOL&&)               = default ;
    EdiNRemoveEOL& EdiNRemoveEOL::operator=(EdiNRemoveEOL&&)    = default ;

    void EdiNRemoveEOL::sprocess(unsigned int repeat_num) const {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_END) ;
            keybrd::pushup(KEYCODE_DELETE) ;
        }) ;
    }
    void EdiNRemoveEOL::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void EdiNRemoveEOL::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }
}