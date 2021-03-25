#include "edi_layout.hpp"

#include "io/keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "time/keystroke_repeater.hpp"
#include "utility.hpp"

namespace vind
{
    struct EdiNRemoveEOL::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNRemoveEOL::EdiNRemoveEOL()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNRemoveEOL::~EdiNRemoveEOL() noexcept                    = default ;
    EdiNRemoveEOL::EdiNRemoveEOL(EdiNRemoveEOL&&)               = default ;
    EdiNRemoveEOL& EdiNRemoveEOL::operator=(EdiNRemoveEOL&&)    = default ;

    const std::string EdiNRemoveEOL::sname() noexcept {
        return "edi_n_remove_EOL" ;
    }
    void EdiNRemoveEOL::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        auto remove = [] {
            keybrd::pushup(VKC_END) ;
            keybrd::pushup(VKC_DELETE) ;
        } ;

        using mode::change_mode ;
        using mode::Mode ;

        if(repeat_num == 1) {
            if(first_call) {
                pimpl->ksr.reset() ;
                remove() ;
            }
            else if(pimpl->ksr.is_pressed()) {
                remove() ;
            }
            return ;
        }

        //repeat_num >= 2
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++)
                remove() ;

            pimpl->ksr.reset() ;
        }
    }
}
