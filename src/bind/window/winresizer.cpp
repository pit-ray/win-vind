#include "winresizer.hpp"

#include <stdexcept>
#include <unordered_map>
#include <windows.h>

#include "resize_win.hpp"
#include "select_win.hpp"
#include "winutil.hpp"

#include "bind/bindinglist.hpp"
#include "bind/emu/movecaret.hpp"
#include "core/background.hpp"
#include "core/entry.hpp"
#include "core/funcfinder.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/optionlist.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "util/constant_accelerator.hpp"
#include "util/def.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/screen_metrics.hpp"
#include "util/winwrap.hpp"

namespace
{
    enum class InnerMode : unsigned char {
        RESIZE,
        MOVE,
        FOCUS,
        NUM,
   } ;
}

namespace vind
{
    namespace bind
    {
        struct WindowResizer::Impl {
            std::size_t left_id_ ;
            std::size_t right_id_ ;
            std::size_t up_id_ ;
            std::size_t down_id_ ;

            core::Background bg_ ;

            explicit Impl()
            : left_id_(MoveCaretLeft().id()),
              right_id_(MoveCaretRight().id()),
              up_id_(MoveCaretUp().id()),
              down_id_(MoveCaretDown().id()),
              bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::Dedicate2Window().name(),
                    opt::SuppressForVim().name(),
                    opt::VCmdLine().name()
              ))
            {}

            void do_resize(std::size_t id) {
                if(id == left_id_) {
                    DecreaseWindowWidth::sprocess(1, "") ;
                }
                else if(id == right_id_) {
                    IncreaseWindowWidth::sprocess(1, "") ;
                }
                else if(id == up_id_) {
                    DecreaseWindowHeight::sprocess(1, "") ;
                }
                else if(id == down_id_) {
                    IncreaseWindowHeight::sprocess(1, "") ;
                }
            }

            void do_move(std::size_t id) {
                auto hwnd = util::get_foreground_window() ;
                auto rect = util::get_window_rect(hwnd) ;

                auto cb_rect = util::get_combined_metrics() ;

                auto left = rect.left() ;
                auto top = rect.top() ;

                auto& settable = core::SetTable::get_instance() ;
                auto delta = settable.get("window_velocity").get<long>() ;

                if(id == left_id_) {
                    left -= delta ;
                    if(left < cb_rect.left()) {
                        left = cb_rect.left() ;
                    }
                }
                else if(id == right_id_) {
                    left += delta ;
                    if(left > cb_rect.right()) {
                        left = cb_rect.right() ;
                    }
                }
                else if(id == up_id_) {
                    top -= delta ;
                    if(top < cb_rect.top()) {
                        top = cb_rect.top() ;
                    }
                }
                else if(id == down_id_) {
                    top += delta ;
                    if(top > cb_rect.bottom()) {
                        top = cb_rect.bottom() ;
                    }
                }

                if(!MoveWindow(hwnd, left, top, rect.width(), rect.height(), TRUE)) {
                    throw RUNTIME_EXCEPT("Could not move the foreground window.") ;
                }
            }

            void do_focus(std::size_t id) {
                if(id == left_id_) {
                    SelectLeftWindow::sprocess(1, "") ;
                }
                else if(id == right_id_) {
                    SelectRightWindow::sprocess(1, "") ;
                }
                else if(id == up_id_) {
                    SelectUpperWindow::sprocess(1, "") ;
                }
                else if(id == down_id_) {
                    SelectLowerWindow::sprocess(1, "") ;
                }
            }

            void call_op(InnerMode mode, std::size_t id) {
                if(mode == InnerMode::RESIZE) {
                    do_resize(id) ;
                }
                else if(mode == InnerMode::MOVE) {
                    do_move(id) ;
                }
                else if(mode == InnerMode::FOCUS) {
                    do_focus(id) ;
                }
            }

            void draw_mode_status(InnerMode mode) const {
                static const std::unordered_map<InnerMode, opt::StaticMessage> lc_msgs = {
                    {InnerMode::RESIZE, "[Resize]... \"Esc\": OK, \"e\": Change mode"},
                    {InnerMode::MOVE,   "[Move]... \"Esc\": OK, \"e\": Change mode"},
                    {InnerMode::FOCUS,  "[Focus]... \"Esc\": OK, \"e\": Change mode"}
                } ;

                opt::VCmdLine::print(lc_msgs.at(mode)) ;
            }

            template <typename T>
            static InnerMode cvt_modulo(T mode) {
                return static_cast<InnerMode>(
                        static_cast<int>(mode) % static_cast<int>(InnerMode::NUM)) ;
            }
        } ;

        WindowResizer::WindowResizer()
        : BindedFuncVoid("window_resizer"),
          pimpl(std::make_unique<Impl>())
        {}

        WindowResizer::~WindowResizer() noexcept                = default ;
        WindowResizer::WindowResizer(WindowResizer&&)            = default ;
        WindowResizer& WindowResizer::operator=(WindowResizer&&) = default ;

        void WindowResizer::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            using core::Mode ;
            auto& igate = core::InputGate::get_instance() ;
            auto& ihub = core::InputHub::get_instance() ;
            auto& settable = core::SetTable::get_instance() ;

            auto inmode = Impl::cvt_modulo(
                    settable.get("winresizer_initmode").get<int>()) ;
            pimpl->draw_mode_status(inmode) ;

            core::InstantKeyAbsorber ika ;
            while(true) {
                pimpl->bg_.update() ;

                if(igate.is_pressed(KEYCODE_ESC) || igate.is_pressed(KEYCODE_ENTER)) {
                    break ;
                }

                if(igate.is_pressed(KEYCODE_E)) { //mode change
                    inmode = Impl::cvt_modulo(static_cast<int>(inmode) + 1) ;
                    pimpl->draw_mode_status(inmode) ;

                    // Release key state for processing at regular intervals.
                    // release_virtually is more efficient and simpler.
                    igate.release_virtually(KEYCODE_E) ;
                    continue ;
                }

                std::vector<core::CmdUnit::SPtr> inputs ;
                std::vector<std::uint16_t> counts ;
                if(!ihub.fetch_inputs(inputs, counts, Mode::EDI_NORMAL)) {
                    continue ;
                }

                for(int i = 0 ; i < inputs.size() ; i ++) {
                    pimpl->call_op(inmode, inputs[i]->id()) ;
                }
            }

            igate.release_virtually(KEYCODE_ESC) ;
            igate.release_virtually(KEYCODE_ENTER) ;
            opt::VCmdLine::reset() ;
        }
    }
}
