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
            core::FuncFinder funcfinder_ ;

            std::size_t left_id_ ;
            std::size_t right_id_ ;
            std::size_t up_id_ ;
            std::size_t down_id_ ;

            util::KeyStrokeRepeater ksr_ ;

            util::ConstAccelerator ca_ ;

            core::Background bg_ ;

            explicit Impl()
            : funcfinder_(),
              left_id_(MoveCaretLeft().id()),
              right_id_(MoveCaretRight().id()),
              up_id_(MoveCaretUp().id()),
              down_id_(MoveCaretDown().id()),
              ksr_(),
              ca_(),
              bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::Dedicate2Window().name(),
                    opt::SuppressForVim().name(),
                    opt::VCmdLine().name()
              ))
            {}

            void do_resize(std::size_t id, bool first_call) {
                if(first_call) {
                    ksr_.reset() ;
                }
                else if(!ksr_.is_passed()) {
                    return ;
                }

                auto& settable = core::SetTable::get_instance() ;

                if(id == left_id_) {
                    DecreaseWindowWidth::sprocess(
                            settable.get("window_hdelta").get<long>()) ;
                }
                else if(id == right_id_) {
                    IncreaseWindowWidth::sprocess(
                            settable.get("window_hdelta").get<long>()) ;
                }
                else if(id == up_id_) {
                    DecreaseWindowHeight::sprocess(
                            settable.get("window_vdelta").get<long>()) ;
                }
                else if(id == down_id_) {
                    IncreaseWindowHeight::sprocess(
                            settable.get("window_vdelta").get<long>()) ;
                }
            }

            void do_move(std::size_t id, bool first_call) {
                if(first_call) {
                    ca_.reset() ;
                }

                auto hwnd = util::get_foreground_window() ;
                auto rect = util::get_window_rect(hwnd) ;

                auto cb_rect = util::get_combined_metrics() ;

                auto left = rect.left() ;
                auto top = rect.top() ;

                if(id == left_id_) {
                    left -= ca_.delta<long>() ;
                    if(left < cb_rect.left()) {
                        left = cb_rect.left() ;
                    }
                }
                else if(id == right_id_) {
                    left += ca_.delta<long>() ;
                    if(left > cb_rect.right()) {
                        left = cb_rect.right() ;
                    }
                }
                else if(id == up_id_) {
                    top -= ca_.delta<long>() ;
                    if(top < cb_rect.top()) {
                        top = cb_rect.top() ;
                    }
                }
                else if(id == down_id_) {
                    top += ca_.delta<long>() ;
                    if(top > cb_rect.bottom()) {
                        top = cb_rect.bottom() ;
                    }
                }

                if(!MoveWindow(hwnd, left, top, rect.width(), rect.height(), TRUE)) {
                    throw RUNTIME_EXCEPT("Could not move the foreground window.") ;
                }
            }

            void do_focus(std::size_t id, bool first_call) {
                if(first_call) {
                    ksr_.reset() ;
                }
                else if(!ksr_.is_passed()) {
                    return ;
                }

                if(id == left_id_) {
                    SelectLeftWindow::sprocess() ;
                }
                else if(id == right_id_) {
                    SelectRightWindow::sprocess() ;
                }
                else if(id == up_id_) {
                    SelectUpperWindow::sprocess() ;
                }
                else if(id == down_id_) {
                    SelectLowerWindow::sprocess() ;
                }
            }

            void call_op(InnerMode mode, std::size_t id, bool first_call) {
                switch(mode) {
                    case InnerMode::RESIZE:
                        do_resize(id, first_call) ;
                        break ;

                    case InnerMode::MOVE:
                        do_move(id, first_call) ;
                        break ;

                    case InnerMode::FOCUS:
                        do_focus(id, first_call) ;
                        break ;

                    default:
                        throw LOGIC_EXCEPT("Unknown id") ;
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

        void WindowResizer::reconstruct() {
            pimpl->funcfinder_.reconstruct(
                core::Mode::EDI_NORMAL,
                ref_global_funcs_bynames(
                    MoveCaretLeft().name(),
                    MoveCaretRight().name(),
                    MoveCaretUp().name(),
                    MoveCaretDown().name()
                )
            ) ;

            auto& settable = core::SetTable::get_instance() ;

            pimpl->ca_.set_acceleration(
                    settable.get("window_accel").get<float>()) ;
            pimpl->ca_.set_max_velocity(
                    settable.get("window_maxv").get<float>()) ;
            pimpl->ca_.set_time_weight(
                    settable.get("window_tweight").get<int>()) ;
        }

        void WindowResizer::sprocess() {
            core::InstantKeyAbsorber ika ;

            pimpl->funcfinder_.reset_parser_states() ;

            core::NTypeLogger lgr ;

            auto& settable = core::SetTable::get_instance() ;

            auto inmode = Impl::cvt_modulo(
                    settable.get("winresizer_initmode").get<int>()) ;

            pimpl->draw_mode_status(inmode) ;

            auto& igate = core::InputGate::get_instance() ;

            std::size_t actid = 0 ;
            while(true) {
                pimpl->bg_.update() ;

                auto log = igate.pop_log() ;
                if(!NTYPE_LOGGED(lgr.logging_state(log))) {
                    continue ;
                }

                if(lgr.is_long_pressing()) {
                    if(actid != 0) {
                        pimpl->call_op(inmode, actid, false) ;
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
                if(lgr.latest().is_containing(KEYCODE_E)) { //mode change
                    lgr.accept() ;
                    pimpl->funcfinder_.reset_parser_states() ;
                    inmode = Impl::cvt_modulo(static_cast<int>(inmode) + 1) ;
                    pimpl->draw_mode_status(inmode) ;
                    continue ;
                }

                if(auto parser = pimpl->funcfinder_.find_parser_with_transition(
                            lgr.latest(), id())) {

                    decltype(auto) id = parser->get_func()->id() ;

                    if(parser->is_accepted()) {
                        actid = id ;

                        lgr.accept() ;
                        pimpl->funcfinder_.reset_parser_states() ;

                        pimpl->call_op(inmode, id, true) ;
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
            opt::VCmdLine::reset() ;
        }
        void WindowResizer::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void WindowResizer::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
