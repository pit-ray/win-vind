#include "window_resizer.hpp"

#include <stdexcept>
#include <unordered_map>
#include <windows.h>

#include "resize_window.hpp"
#include "select_window.hpp"
#include "window_utility.hpp"

#include "bind/bindings_lists.hpp"
#include "bind/emu/edi_move_caret.hpp"
#include "core/background.hpp"
#include "core/entry.hpp"
#include "core/func_finder.hpp"
#include "core/g_params.hpp"
#include "core/key_absorber.hpp"
#include "core/logpooler.hpp"
#include "core/ntype_logger.hpp"
#include "opt/async_uia_cache_builder.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/optionlist.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/vcmdline.hpp"
#include "util/constant_accelerator.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/screen_metrics.hpp"

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

                if(id == left_id_) {
                    DecreaseWindowWidth::sprocess(core::get_l("window_hdelta")) ;
                }
                else if(id == right_id_) {
                    IncreaseWindowWidth::sprocess(core::get_l("window_hdelta")) ;
                }
                else if(id == up_id_) {
                    DecreaseWindowHeight::sprocess(core::get_l("window_vdelta")) ;
                }
                else if(id == down_id_) {
                    IncreaseWindowHeight::sprocess(core::get_l("window_vdelta")) ;
                }
            }

            void do_move(std::size_t id, bool first_call) {
                if(first_call) {
                    ca_.reset() ;
                }

                auto hwnd = GetForegroundWindow() ;

                util::Box2D rect ;
                if(!GetWindowRect(hwnd, &(rect.data()))) {
                    throw RUNTIME_EXCEPT("Could not get a rectangle of foreground window.") ;
                }

                auto cb_rect = util::get_conbined_metrics() ;

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
                ref_global_funcs_bynames(
                    MoveCaretLeft().name(),
                    MoveCaretRight().name(),
                    MoveCaretUp().name(),
                    MoveCaretDown().name()
            )) ;
            pimpl->ca_.set_acceleration(core::get_f("window_accel")) ;
            pimpl->ca_.set_max_velocity(core::get_f("window_maxv")) ;
            pimpl->ca_.set_time_weight(core::get_i("window_tweight")) ;
        }

        void WindowResizer::sprocess() const {
            constexpr auto lcx_vmode = core::Mode::EDI_NORMAL ;

            core::InstantKeyAbsorber ika ;

            pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;

            core::NTypeLogger lgr ;

            auto inmode = Impl::cvt_modulo(core::get_i("winresizer_initmode")) ;

            pimpl->draw_mode_status(inmode) ;

            std::size_t actid = 0 ;
            while(true) {
                pimpl->bg_.update() ;

                auto log = core::LogPooler::get_instance().pop_log() ;
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
                    pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;
                    inmode = Impl::cvt_modulo(static_cast<int>(inmode) + 1) ;
                    pimpl->draw_mode_status(inmode) ;
                    continue ;
                }

                if(auto parser = pimpl->funcfinder_.find_parser_with_transition(
                            lgr.latest(), id(), lcx_vmode)) {

                    decltype(auto) id = parser->get_func()->id() ;

                    if(parser->is_accepted()) {
                        actid = id ;

                        lgr.accept() ;
                        pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;

                        pimpl->call_op(inmode, id, true) ;
                        continue ;
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
            opt::VCmdLine::reset() ;
        }
        void WindowResizer::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void WindowResizer::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess() ;
        }
    }
}
