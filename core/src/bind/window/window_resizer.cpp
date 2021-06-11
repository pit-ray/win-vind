#include "bind/window/window_resizer.hpp"

#include <windows.h>

#include "entry.hpp"
#include "g_params.hpp"

#include "bind/func_finder.hpp"
#include "io/keybrd.hpp"
#include "io/screen_metrics.hpp"
#include "key/key_absorber.hpp"
#include "key/ntype_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"

#include "bind/window/window_utility.hpp"

#include "bind/emu/edi_move_caret.hpp"
#include "bind/window/resize_window.hpp"
#include "bind/window/select_window.hpp"

namespace
{
    enum class InnerMode : unsigned char {
        RESIZE,
        MOVE,
        FOCUS,
        NUM,
   } ;
}

#include <chrono>
#include "util/math.hpp"

namespace
{
    using namespace vind ;

    using namespace std::chrono ;
    //hardcoded (The cursor move 1px necessarilly by one press)
    constexpr auto INITIAL_VELOCITY = 1.0f ;

    template <typename T>
    inline auto constant_accelerate(float& velocity, T&& us) {
        auto acc = gparams::get_f("cursor_acceleration") ;
        auto mvc = gparams::get_f("cursor_max_velocity") ;

        constexpr auto TIME_COEF = util::pow_f(10, -3) ;
        auto t = us * TIME_COEF / gparams::get_i("cursor_weight") ; //accuracy
        auto x = velocity*t + 0.5f*acc*t*t ;
        auto delta_v = acc * t ;
        if(velocity + delta_v < mvc) velocity += delta_v ;
        else velocity = mvc ;
        return x ;
    }

    inline auto compute_delta_t(
            const system_clock::time_point& start_time) {

        return duration_cast<microseconds>(
                system_clock::now() - start_time).count() ;
    }

    class MoveDeltaCalculator {
    private:
        float v_ = INITIAL_VELOCITY ;
        system_clock::time_point start_time_ = system_clock::now() ;

    public:
        void reset() noexcept {
            v_ = INITIAL_VELOCITY ;
            start_time_ = system_clock::now() ;
        }

        template <typename T>
        auto delta() {
            return static_cast<T>(
                    constant_accelerate(v_, compute_delta_t(start_time_))) ;
        }
    } ;
}

namespace vind
{
    struct WindowResizer::Impl {
        FuncFinder funcfinder_ ;

        std::size_t left_id_ ;
        std::size_t right_id_ ;
        std::size_t up_id_ ;
        std::size_t down_id_ ;

        KeyStrokeRepeater ksr_ ;

        MoveDeltaCalculator calcer_ ;

        explicit Impl()
        : funcfinder_(),
          left_id_(MoveCaretLeft().id()),
          right_id_(MoveCaretRight().id()),
          up_id_(MoveCaretUp().id()),
          down_id_(MoveCaretDown().id()),
          ksr_(),
          calcer_()
        {}

        void do_resize(std::size_t id, bool first_call) {
            if(first_call) {
                ksr_.reset() ;
            }
            else if(!ksr_.is_pressed()) {
                return ;
            }

            if(id == left_id_) {
                DecreaseWindowWidth::sprocess(gparams::get_l("window_width_delta")) ;
            }
            else if(id == right_id_) {
                IncreaseWindowWidth::sprocess(gparams::get_l("window_width_delta")) ;
            }
            else if(id == up_id_) {
                DecreaseWindowHeight::sprocess(gparams::get_l("window_height_delta")) ;
            }
            else if(id == down_id_) {
                IncreaseWindowHeight::sprocess(gparams::get_l("window_height_delta")) ;
            }
        }

        void do_move(std::size_t id, bool first_call) {
            if(first_call) {
                calcer_.reset() ;
            }

            auto hwnd = GetForegroundWindow() ;

            RECT rect ;
            if(!GetWindowRect(hwnd, &rect)) {
                throw RUNTIME_EXCEPT("Could not get a rectangle of foreground window.") ;
            }

            RECT cb_rect ;
            screenmetrics::get_conbined_metrics(&cb_rect) ;

            auto left = rect.left ;
            auto top = rect.top ;

            if(id == left_id_) {
                left -= calcer_.delta<long>() ;
                if(left < cb_rect.left) {
                    left = cb_rect.left ;
                }
            }
            else if(id == right_id_) {
                left += calcer_.delta<long>() ;
                if(left > cb_rect.right) {
                    left = cb_rect.right ;
                }
            }
            else if(id == up_id_) {
                top -= calcer_.delta<long>() ;
                if(top < cb_rect.top) {
                    top = cb_rect.top ;
                }
            }
            else if(id == down_id_) {
                top += calcer_.delta<long>() ;
                if(top > cb_rect.bottom) {
                    top = cb_rect.bottom ;
                }
            }

            if(!MoveWindow(
                        hwnd, left, top,
                        screenmetrics::width(rect),
                        screenmetrics::height(rect), TRUE)) {
                throw RUNTIME_EXCEPT("Could not move the foreground window.") ;
            }
        }

        void do_focus(std::size_t id, bool first_call) {
            if(first_call) {
                ksr_.reset() ;
            }
            else if(!ksr_.is_pressed()) {
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

        bool is_valid_id(std::size_t id) const noexcept {
            return id == left_id_ || id == right_id_ \
                       || id == up_id_ || id == down_id_ ;
        }

        void draw_mode_info(InnerMode mode) const {
            switch(mode) {
                case InnerMode::RESIZE:
                    VirtualCmdLine::cout("[Resize]... \"Esc\": OK, \"e\": Change mode") ;
                    break ;

                case InnerMode::MOVE:
                    VirtualCmdLine::cout("[Move]... \"Esc\": OK, \"e\": Change mode") ;
                    break ;

                case InnerMode::FOCUS:
                    VirtualCmdLine::cout("[Focus]... \"Esc\": OK, \"e\": Change mode") ;
                    break ;

                default:
                    break ;
            }
            VirtualCmdLine::refresh() ;
        }
    } ;

    WindowResizer::WindowResizer()
    : BindedFuncCreator("window_resizer"),
      pimpl(std::make_unique<Impl>())
    {}

    WindowResizer::~WindowResizer() noexcept                = default ;
    WindowResizer::WindowResizer(WindowResizer&&)            = default ;
    WindowResizer& WindowResizer::operator=(WindowResizer&&) = default ;

    void WindowResizer::reconstruct() {
        pimpl->funcfinder_.reconstruct_funcset() ;
    }

    void WindowResizer::sprocess() const {
        using namespace keybrd ;

        constexpr auto lcx_vmode = mode::Mode::EDI_NORMAL ;

        keyabsorber::InstantKeyAbsorber ika ;

        pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;

        NTypeLogger lgr ;
        std::size_t actid = 0 ;
        auto inmode = InnerMode::RESIZE ;
        pimpl->draw_mode_info(inmode) ;
        while(vind::update_background()) {
            if(!NTYPE_LOGGED(lgr.logging_state())) {
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
                inmode = static_cast<InnerMode>(
                        (static_cast<int>(inmode) + 1) % static_cast<int>(InnerMode::NUM)) ;
                pimpl->draw_mode_info(inmode) ;
                continue ;
            }

            if(auto parser = pimpl->funcfinder_.find_parser_with_transition(
                        lgr.latest(), id(), lcx_vmode)) {

                decltype(auto) id = parser->get_func()->id() ;

                if(parser->is_accepted()) {
                    if(pimpl->is_valid_id(id)) {
                        actid = id ;

                        lgr.accept() ;
                        pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;

                        pimpl->call_op(inmode, id, true) ;
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

        keyabsorber::release_virtually(KEYCODE_ESC) ;
        keyabsorber::release_virtually(KEYCODE_ENTER) ;
        VirtualCmdLine::reset() ;
    }
    void WindowResizer::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void WindowResizer::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
}
