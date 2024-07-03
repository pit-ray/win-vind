#include "background.hpp"

#include "exception.hpp"
#include "inputgate.hpp"

#include "opt/option.hpp"
#include "opt/optionlist.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace core
    {
        struct Background::Impl {
            std::vector<opt::Option::SPtr> opts_ ;
            bool screen_is_locked_ ;

            template <typename T>
            Impl(T&& opts)
            : opts_(std::forward<T>(opts)),
              screen_is_locked_(false)
            {}

            bool is_screen_locked() {
                // GetCursorPos fails when the screen is locked.
                POINT pos ;
                return GetCursorPos(&pos) == 0 ;
            }
        } ;

        Background::Background()
        : pimpl(std::make_unique<Impl>(opt::all_global_options()))
        {}

        Background::Background(const std::vector<opt::Option::SPtr>& opts)
        : pimpl(std::make_unique<Impl>(opts))
        {}

        Background::Background(std::vector<opt::Option::SPtr>&& opts)
        : pimpl(std::make_unique<Impl>(std::move(opts)))
        {}

        Background::~Background() noexcept = default ;

        bool Background::update() {
            auto& igate = InputGate::get_instance() ;
            Sleep(5) ;

            if(pimpl->is_screen_locked()) {
                if(!pimpl->screen_is_locked_) {
                    // Release all keys when the screen is locked.
                    for(auto& key : igate.pressed_list()) {
                        igate.release_virtually(key) ;
                    }
                    pimpl->screen_is_locked_ = true ;
                }
                return false ;
            }
            else if(pimpl->screen_is_locked_) {
                pimpl->screen_is_locked_ = false ;
            }

            util::get_win_message() ;

            for(const auto& op : pimpl->opts_) {
                op->process() ;
            }

            igate.refresh_toggle_state() ;

            if(igate.is_really_pressed(KEYCODE_F8) && \
               igate.is_really_pressed(KEYCODE_F9)) {
                throw SafeForcedTermination() ;
            }
            return true ;
        }
    }
}
