#include "switch_win.hpp"

#include <memory>
#include <windows.h>

#include "bind/bindinglist.hpp"
#include "bind/emu/movecaret.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "core/background.hpp"
#include "core/entry.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/mode.hpp"
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
            std::size_t left_id_ ;
            std::size_t right_id_ ;

            core::Background bg_ ;

            explicit Impl()
            : left_id_(MoveCaretLeft().id()),
              right_id_(MoveCaretRight().id()),
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

        void SwitchWindow::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            using core::Mode ;
            auto& igate = core::InputGate::get_instance() ;
            auto& ihub = core::InputHub::get_instance() ;

            core::InstantKeyAbsorber ika ;

            core::ScopedKey alt(KEYCODE_LALT) ;
            alt.press() ;
            igate.release_virtually(KEYCODE_LALT) ;
            igate.pushup(KEYCODE_TAB) ;

            while(true) {
                pimpl->bg_.update() ;

                bool break_flag = false ;
                do {
                    core::CmdUnit::SPtr input ;
                    std::uint16_t count ;
                    if(!ihub.pull_input(
                            input, count, Mode::EDI_NORMAL, false)) {
                        continue ;
                    }

                    if(input->is_containing(KEYCODE_ESC) ||
                            input->is_containing(KEYCODE_ENTER)) {
                        break_flag = true ;
                        break ;
                    }

                    pimpl->call_op(input->id()) ;
                    Sleep(100) ;
                } while(!ihub.is_empty_queue()) ;

                if(break_flag) {
                    break ;
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
