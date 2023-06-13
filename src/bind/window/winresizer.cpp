#include "winresizer.hpp"

#include <stdexcept>
#include <unordered_map>
#include <windows.h>

#include "resize_win.hpp"
#include "select_win.hpp"
#include "winutil.hpp"

#include "bind/bindinglist.hpp"
#include "bind/mouse/movecursor.hpp"
#include "core/background.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/keycodedef.hpp"
#include "core/mapsolver.hpp"
#include "core/settable.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/optionlist.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
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

    enum class OperationID : unsigned char {
        NONE,
        LEFT,
        RIGHT,
        UP,
        DOWN,
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
            : left_id_(MoveCursorLeft().id()),
              right_id_(MoveCursorRight().id()),
              up_id_(MoveCursorUp().id()),
              down_id_(MoveCursorDown().id()),
              bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::Dedicate2Window().name(),
                    opt::SuppressForVim().name(),
                    opt::VCmdLine().name()
              ))
            {}

            void do_resize(OperationID id) {
                switch(id) {
                    case OperationID::LEFT:
                        DecreaseWindowWidth::sprocess(1, "") ;
                        break ;

                    case OperationID::RIGHT:
                        IncreaseWindowWidth::sprocess(1, "") ;
                        break ;

                    case OperationID::UP:
                        DecreaseWindowHeight::sprocess(1, "") ;
                        break ;

                    case OperationID::DOWN:
                        IncreaseWindowHeight::sprocess(1, "") ;
                        break ;

                    default:
                        break ;
                }
            }

            void do_move(OperationID id) {
                auto hwnd = util::get_foreground_window() ;
                auto rect = util::get_window_rect(hwnd) ;

                auto cb_rect = util::get_combined_metrics() ;

                auto left = rect.left() ;
                auto top = rect.top() ;

                auto& settable = core::SetTable::get_instance() ;
                auto delta = settable.get("window_velocity").get<long>() ;

                switch(id) {
                    case OperationID::LEFT:
                        left -= delta ;
                        if(left < cb_rect.left()) {
                            left = cb_rect.left() ;
                        }
                        break;

                    case OperationID::RIGHT:
                        left += delta ;
                        if(left > cb_rect.right()) {
                            left = cb_rect.right() ;
                        }
                        break;

                    case OperationID::UP:
                        top -= delta ;
                        if(top < cb_rect.top()) {
                            top = cb_rect.top() ;
                        }
                        break;

                    case OperationID::DOWN:
                        top += delta ;
                        if(top > cb_rect.bottom()) {
                            top = cb_rect.bottom() ;
                        }
                        break;

                    default:
                        break ;
                }

                if(!MoveWindow(hwnd, left, top, rect.width(), rect.height(), TRUE)) {
                    throw RUNTIME_EXCEPT("Could not move the foreground window.") ;
                }
            }

            void do_focus(OperationID id) {
                switch(id) {
                    case OperationID::LEFT:
                        SelectLeftWindow::sprocess(1, "") ;
                        break ;

                    case OperationID::RIGHT:
                        SelectRightWindow::sprocess(1, "") ;
                        break ;

                    case OperationID::UP:
                        SelectUpperWindow::sprocess(1, "") ;
                        break ;

                    case OperationID::DOWN:
                        SelectLowerWindow::sprocess(1, "") ;
                        break ;

                    default:
                        break ;
                }
            }

            bool call_op(InnerMode mode, const core::CmdUnit::SPtr& input) {
                auto id = OperationID::NONE ;
                if(input->is_containing(KEYCODE_H)) {
                    id = OperationID::LEFT ;
                }
                else if(input->is_containing(KEYCODE_L)) {
                    id = OperationID::RIGHT ;
                }
                else if(input->is_containing(KEYCODE_K)) {
                    id = OperationID::UP ;
                }
                else if(input->is_containing(KEYCODE_J)) {
                    id = OperationID::DOWN ;
                }

                if(mode == InnerMode::RESIZE) {
                    do_resize(id) ;
                    return true ;
                }
                else if(mode == InnerMode::MOVE) {
                    do_move(id) ;
                    return true ;
                }
                else if(mode == InnerMode::FOCUS) {
                    do_focus(id) ;
                    return true ;
                }
                return false ;
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
            auto& igate = core::InputGate::get_instance() ;
            auto& ihub = core::InputHub::get_instance() ;
            auto& settable = core::SetTable::get_instance() ;

            auto inmode = Impl::cvt_modulo(
                    settable.get("winresizer_initmode").get<int>()) ;
            pimpl->draw_mode_status(inmode) ;

            core::InstantKeyAbsorber ika ;
            while(true) {
                pimpl->bg_.update() ;

                bool break_flag = false ;
                do {
                    core::CmdUnit::SPtr input ;
                    std::uint16_t count ;
                    if(!ihub.get_typed_input(
                            input, count, core::get_global_mode(), false)) {
                        continue ;
                    }

                    if(input->is_containing(KEYCODE_ESC) ||
                            input->is_containing(KEYCODE_ENTER)) {
                        break_flag = true ;
                        break ;
                    }

                    if(input->is_containing(KEYCODE_E)) { //mode change
                        inmode = Impl::cvt_modulo(static_cast<int>(inmode) + 1) ;
                        pimpl->draw_mode_status(inmode) ;

                        // Release key state for processing at regular intervals.
                        // release_virtually is more efficient and simpler.
                        igate.release_virtually(KEYCODE_E) ;
                        continue ;
                    }

                    if(pimpl->call_op(inmode, input)) {
                        continue ;
                    }
                    Sleep(100) ;

                } while(!ihub.is_empty_queue()) ;

                if(break_flag) {
                    break ;
                }
            }

            igate.release_virtually(KEYCODE_ESC) ;
            igate.release_virtually(KEYCODE_ENTER) ;
            opt::VCmdLine::reset() ;
        }
    }
}
