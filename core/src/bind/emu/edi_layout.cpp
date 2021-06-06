#include "bind/emu/edi_layout.hpp"

#include "bind/safe_repeater.hpp"
#include "io/keybrd.hpp"
#include "key/ntype_logger.hpp"
#include "mode.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"


namespace vind
{
    struct JoinNextLine::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    JoinNextLine::JoinNextLine()
    : BindedFuncCreator("join_next_line"),
      pimpl(std::make_unique<Impl>())
    {}

    JoinNextLine::~JoinNextLine() noexcept                    = default ;
    JoinNextLine::JoinNextLine(JoinNextLine&&)               = default ;
    JoinNextLine& JoinNextLine::operator=(JoinNextLine&&)    = default ;

    void JoinNextLine::sprocess(unsigned int repeat_num) const {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_END) ;
            keybrd::pushup(KEYCODE_DELETE) ;
        }) ;
    }
    void JoinNextLine::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void JoinNextLine::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }
}
