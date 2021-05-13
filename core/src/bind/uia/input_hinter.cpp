#include "bind/uia/input_hinter.hpp"

#include <future>
#include <memory>
#include <mutex>

#include "bind/uia/point_2d.hpp"
#include "entry.hpp"
#include "key/char_logger.hpp"
#include "key/key_absorber.hpp"
#include "util/winwrap.hpp"

namespace vind
{
    struct InputHinter::Impl {
        std::vector<unsigned char> matched_counts_{} ;
        std::size_t drawable_hints_num_ = 0 ;
        std::atomic_bool cancel_running_ = false ;
        std::mutex mtx_{} ;

        // return : matched index
        long validate_if_match_with_hints(
                const KeyLoggerBase& lgr,
                const std::vector<Hint>& hints) {

            std::lock_guard<std::mutex> socoped_lock{mtx_} ;

            if(lgr.empty()) {
                drawable_hints_num_ = hints.size() ; //must draw all hints
                return -1 ;
            }

            drawable_hints_num_ = 0 ;

            for(std::size_t i = 0 ; i < hints.size() ; i ++) {

                std::size_t seq_idx ;
                for(seq_idx = 0 ; seq_idx < lgr.size() ; seq_idx ++) {
                    try {
                        if(!lgr.at(seq_idx).is_containing(hints[i].at(seq_idx))) {
                            break ;
                        }
                    }
                    catch(const std::out_of_range&) {
                        break ;
                    }
                }

                if(seq_idx == lgr.size()) {
                    drawable_hints_num_ ++ ;
                    matched_counts_[i] = static_cast<unsigned char>(seq_idx) ;
                }
                else {
                    matched_counts_[i] = 0 ;
                }

                if(seq_idx == hints[i].size()) {
                    return static_cast<long>(i) ;
                }
            }
            return -1 ;
        }
    } ;

    InputHinter::InputHinter()
    : pimpl(std::make_unique<Impl>())
    {
        pimpl->matched_counts_.reserve(512) ;
    }

    InputHinter::~InputHinter() noexcept = default ;

    InputHinter::InputHinter(InputHinter&&)            = default ;
    InputHinter& InputHinter::operator=(InputHinter&&) = default ;


    Point2D::SPtr InputHinter::launch_loop(
            const std::vector<Point2D>& positions,
            const std::vector<Hint>& hints) {

        pimpl->matched_counts_.clear() ;
        pimpl->matched_counts_.resize(positions.size()) ;
        pimpl->drawable_hints_num_ = positions.size() ;
        pimpl->cancel_running_ = false ;

        keyabsorber::InstantKeyAbsorber ika ;
        CharLogger lgr{
            KEYCODE_ESC,
            KEYCODE_BKSPACE
        };

        while(vind::update_background() && !(pimpl->cancel_running_)) {
            if(!CHAR_LOGGED(lgr.logging_state())) {
                continue ;
            }

            if(lgr.latest().is_containing(KEYCODE_ESC)) {
                keyabsorber::release_virtually(KEYCODE_ESC) ;
                return nullptr ;
            }

            if(lgr.latest().is_containing(KEYCODE_BKSPACE)) {
                keyabsorber::release_virtually(KEYCODE_BKSPACE) ;
                if(lgr.size() == 1) {
                    return nullptr ;
                }

                lgr.remove_from_back(2) ;

                pimpl->validate_if_match_with_hints(lgr, hints) ; //update matching list
                continue ;
            }


            auto full_match_idx = pimpl->validate_if_match_with_hints(lgr, hints) ;
            if(full_match_idx >= 0) {
                return std::make_shared<Point2D>(
                        positions[full_match_idx].x(),
                        positions[full_match_idx].y()) ;
            }

            if(pimpl->drawable_hints_num_ == 0) {
                lgr.remove_from_back(1) ;
            }
            else {
                util::refresh_display(NULL) ;
            }
        }
        return nullptr ;
    }

    std::shared_future<Point2D::SPtr> InputHinter::launch_async_loop(
            const std::vector<Point2D>& positions,
            const std::vector<Hint>& hints) {

        auto ft = std::async(
                std::launch::async,
                &InputHinter::launch_loop,
                this,
                std::cref(positions),
                std::cref(hints)) ;

        return ft.share() ;
    }

    void InputHinter::cancel() noexcept {
        pimpl->cancel_running_ = true ;
    }

    const std::vector<unsigned char>& InputHinter::matched_counts() const noexcept {
        return pimpl->matched_counts_ ;
    }

    const std::size_t& InputHinter::drawable_hints_num() const noexcept {
        return pimpl->drawable_hints_num_ ;
    }
}
