#include "switch_win.hpp"

#include <memory>
#include <windows.h>

#include "bind/bindinglist.hpp"
#include "bind/emu/movecaret.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "core/background.hpp"
#include "core/entry.hpp"
#include "core/funcfinder.hpp"
#include "core/inputgate.hpp"
#include "core/keylgrbase.hpp"
#include "core/mode.hpp"
#include "core/ntypelogger.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/optionlist.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"
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

            core::Background bg_ ;

            explicit Impl()
            : funcfinder_(),
              left_id_(MoveCaretLeft().id()),
              right_id_(MoveCaretRight().id()),
              ksr_(),
              bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::Dedicate2Window().name(),
                    opt::SuppressForVim().name(),
                    opt::VCmdLine().name()
              ))
            {}

            void call_op(std::size_t id) const {
                auto& igate = core::InputGate::get_instance() ;
                if(id == left_id_) {
                    igate.pushup(KEYCODE_LEFT) ;
                }
                else if(id == right_id_) {
                    igate.pushup(KEYCODE_RIGHT) ;
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
            /*
            pimpl->funcfinder_.reconstruct(
                core::Mode::EDI_NORMAL,
                ref_global_funcs_bynames(
                    MoveCaretLeft().name(),
                    MoveCaretRight().name()
                )
            ) ;
            */
        }

        void SwitchWindow::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto& igate = core::InputGate::get_instance() ;

            core::InstantKeyAbsorber ika ;

            core::ScopedKey alt(KEYCODE_LALT) ;
            alt.press() ;
            igate.release_virtually(KEYCODE_LALT) ;

            igate.pushup(KEYCODE_TAB) ;

            pimpl->funcfinder_.reset_parser_states() ;

            core::NTypeLogger lgr ;
            std::size_t actid = 0 ;
            while(true) {
                pimpl->bg_.update() ;

                core::KeyLog log{igate.pressed_list().data()} ;
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

                if(auto parser = pimpl->funcfinder_.find_parser_with_transition(lgr.latest(), id())) {

                    decltype(auto) id = parser->get_func()->id() ;

                    if(parser->is_accepted()) {
                        actid = id ;

                        lgr.accept() ;
                        pimpl->funcfinder_.reset_parser_states() ;

                        pimpl->ksr_.reset() ;

                        pimpl->call_op(id) ;
                        continue ;
                    }
                    else if(parser->is_rejected_with_ready()) {
                        lgr.remove_from_back(1) ;
                        pimpl->funcfinder_.backward_parser_states(1) ;
                    }
                }
                else {
                    lgr.reject() ;
                    pimpl->funcfinder_.reset_parser_states() ;
                }
            }

            igate.release_virtually(KEYCODE_ESC) ;
            igate.release_virtually(KEYCODE_ENTER) ;

            alt.release() ;

            //jump cursor to a selected window after releasing alt and tab.
            Sleep(50) ; //send select-message to OS(wait)
            JumpToActiveWindow::sprocess(1, "") ;
        }
    }
}
