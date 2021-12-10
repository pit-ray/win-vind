#include "switch_window.hpp"

#include <memory>
#include <windows.h>

#include "bind/emu/edi_move_caret.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "core/entry.hpp"
#include "core/func_finder.hpp"
#include "core/key_absorber.hpp"
#include "core/key_logger_base.hpp"
#include "core/logpooler.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/keystroke_repeater.hpp"

namespace vind
{
    namespace bind
    {
        struct SwitchWindow::Impl {
            core::FuncFinder funcfinder_ ;
            std::size_t left_id_ ;
            std::size_t right_id_ ;
            util::KeyStrokeRepeater ksr_ ;

            explicit Impl()
            : funcfinder_(),
              left_id_(MoveCaretLeft().id()),
              right_id_(MoveCaretRight().id()),
              ksr_()
            {}

            bool is_valid_id(std::size_t id) const noexcept {
                return id == left_id_ || id == right_id_ ;
            }

            void call_op(std::size_t id) const {
                if(id == left_id_) {
                    util::pushup(KEYCODE_LEFT) ;
                }
                else if(id == right_id_) {
                    util::pushup(KEYCODE_RIGHT) ;
                }
            }
        } ;

        //SwitchWindow
        SwitchWindow::SwitchWindow()
        : BindedFuncVoid("switch_window"),
          pimpl(std::make_unique<Impl>())
        {}
        SwitchWindow::~SwitchWindow() noexcept                = default ;
        SwitchWindow::SwitchWindow(SwitchWindow&&)            = default ;
        SwitchWindow& SwitchWindow::operator=(SwitchWindow&&) = default ;

        void SwitchWindow::reconstruct() {
            pimpl->funcfinder_.reconstruct() ;
        }

        void SwitchWindow::sprocess() const {
            core::InstantKeyAbsorber ika ;

            util::ScopedKey alt(KEYCODE_LALT) ;
            alt.press() ;
            core::release_virtually(KEYCODE_LALT) ;

            util::pushup(KEYCODE_TAB) ;

            constexpr auto lcx_vmode = core::Mode::EDI_NORMAL ;

            pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;

            core::NTypeLogger lgr ;
            std::size_t actid = 0 ;
            while(core::update_background()) {
                auto log = core::LogPooler::get_instance().pop_log() ;
                if(!NTYPE_LOGGED(lgr.logging_state(log))) {
                    continue ;
                }

                if(lgr.is_long_pressing()) {
                    if(pimpl->ksr_.is_passed()) {
                        pimpl->call_op(actid) ;
                    }
                    continue ;
                }
                actid = 0 ;

                if(lgr.latest().is_containing(KEYCODE_ESC)) {
                    break ;
                }
                if(lgr.latest().is_containing(KEYCODE_ENTER)) {
                    break ;
                }

                if(auto parser = pimpl->funcfinder_.find_parser_with_transition(
                            lgr.latest(), id(), lcx_vmode)) {

                    decltype(auto) id = parser->get_func()->id() ;

                    if(parser->is_accepted()) {
                        if(pimpl->is_valid_id(id)) {
                            actid = id ;

                            lgr.accept() ;
                            pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;

                            pimpl->ksr_.reset() ;

                            pimpl->call_op(id) ;
                            continue ;
                        }
                        else {
                            lgr.reject() ;
                            pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;
                        }
                    }
                    else if(parser->is_rejected_with_ready()) {
                        lgr.remove_from_back(1) ;
                        pimpl->funcfinder_.backward_parser_states(1, lcx_vmode) ;
                    }
                }
                else {
                    lgr.reject() ;
                    pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;
                }
            }

            core::release_virtually(KEYCODE_ESC) ;
            core::release_virtually(KEYCODE_ENTER) ;

            alt.release() ;

            //jump cursor to a selected window after releasing alt and tab.
            Sleep(50) ; //send select-message to OS(wait)
            JumpToActiveWindow::sprocess() ;
        }
        void SwitchWindow::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void SwitchWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess() ;
        }
    }
}
