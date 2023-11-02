#include "switch_win.hpp"

#include <memory>
#include <windows.h>

#include "bind/bindinglist.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "core/background.hpp"
#include "core/entry.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/mode.hpp"
#include "opt/optionlist.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"
#include "util/keystroke_repeater.hpp"

namespace vind
{
    namespace bind
    {
        struct SwitchWindow::Impl {
            core::Background bg_ ;

            explicit Impl()
            : bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::VCmdLine().name()
              ))
            {}
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

            // Wait until the switching control is shown.
            Sleep(200) ;

            auto mode = core::get_global_mode() ;

            while(true) {
                pimpl->bg_.update() ;

                bool break_flag = false ;
                do {
                    core::CmdUnit::SPtr input ;
                    std::uint16_t count ;
                    if(!ihub.fetch_input(input, count, mode, false)) {
                        continue ;
                    }

                    if(input->is_containing(KEYCODE_ESC) ||
                            input->is_containing(KEYCODE_ENTER)) {
                        break_flag = true ;
                        break ;
                    }

                    if(input->is_containing(KEYCODE_H)) {
                        igate.pushup(KEYCODE_LEFT) ;
                        continue ;
                    }
                    if(input->is_containing(KEYCODE_L)) {
                        igate.pushup(KEYCODE_RIGHT) ;
                        continue ;
                    }

                    // Map the inputs except for h and l.
                    if(!ihub.enqueue_mapped(input, count, mode)) {
                        continue ;
                    }
                    if(!ihub.fetch_input(input, count, mode, false)) {
                        continue ;
                    }

                    input->execute(count) ;
                    break_flag = true ;  // executed some commands.
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

            ihub.clear_queue() ;
        }
    }
}
