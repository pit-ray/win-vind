#include "bind/window/window_resizer.hpp"


namespace vind
{
    struct WindowResizer::Impl {
        FuncFinder funcfinder_ ;
        std::size_t left_id_ ;
        std::size_t right_id_ ;
        std::size_t active_id_ ;
        KeyStrokeRepeater ksr_ ;

        explicit Impl()
        : funcfinder_(),
          left_id_(MoveCaretLeft().id()),
          right_id_(MoveCaretRight().id()),
          active_id_(0),
          ksr_()
        {}

        bool is_valid_id(std::size_t id) const noexcept {
            return id == left_id_ || id == right_id_ ;
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

        keyabsorber::InstantKeyAbsorber ika ;

        ScopedKey alt(KEYCODE_LALT) ;
        alt.press() ;
        keyabsorber::release_virtually(KEYCODE_LALT) ;

        pushup(KEYCODE_TAB) ;

        constexpr auto lcx_vmode = mode::Mode::EDI_NORMAL ;

        pimpl->funcfinder_.reset_parser_states(lcx_vmode) ;

        NTypeLogger lgr ;
        while(vind::update_background()) {
            if(!NTYPE_LOGGED(lgr.logging_state())) {
                continue ;
            }

            if(lgr.is_long_pressing()) {
                if(pimpl->ksr_.is_pressed()) {
                    pimpl->call_op(pimpl->active_id_) ;
                }
                continue ;
            }
            pimpl->active_id_ = 0 ;

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
                        pimpl->active_id_ = id ;

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

        keyabsorber::release_virtually(KEYCODE_ESC) ;
        keyabsorber::release_virtually(KEYCODE_ENTER) ;

        alt.release() ;

        //jump cursor to a selected window after releasing alt and tab.
        Sleep(50) ; //send select-message to OS(wait)
        JumpToActiveWindow::sprocess() ;
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

}
