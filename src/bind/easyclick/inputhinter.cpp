#include "inputhinter.hpp"

#include <future>
#include <memory>
#include <mutex>

#include "core/background.hpp"
#include "core/charlogger.hpp"
#include "core/inputgate.hpp"
#include "opt/optionlist.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"

namespace vind
{
    namespace bind
    {
        struct InputHinter::Impl {
            std::vector<unsigned char> matched_counts_ ;
            std::size_t drawable_hints_num_ ;
            std::atomic<bool> cancel_running_ ;
            std::mutex mtx_ ;

            core::Background bg_ ;

            Impl()
            : matched_counts_(),
              drawable_hints_num_(0),
              cancel_running_(false),
              mtx_(),
              bg_(opt::ref_global_options_bynames(
                  opt::AsyncUIACacheBuilder().name(),
                  opt::VCmdLine().name()
              ))
            {}

            void init(std::size_t all_hints_num) {
                std::lock_guard<std::mutex> socoped_lock{mtx_} ;
                matched_counts_.clear() ;
                matched_counts_.resize(all_hints_num) ;
                drawable_hints_num_ = all_hints_num ;
                cancel_running_ = false ;
            }

            // return : matched index
            long validate_if_match_with_hints(
                    const core::KeyLoggerBase& lgr,
                    const std::vector<Hint>& hints) {

                std::lock_guard<std::mutex> socoped_lock{mtx_} ;

                if(lgr.empty()) {
                    drawable_hints_num_ = hints.size() ; //must draw all hints
                    return -1 ;
                }

                drawable_hints_num_ = 0 ;

                for(std::size_t i = 0 ; i < hints.size() ; i ++) {

                    std::size_t seq_idx = 0 ;
                    while(seq_idx < lgr.size()) {
                        try {
                            if(!lgr.at(seq_idx).is_containing(hints[i].at(seq_idx))) {
                                break ;
                            }
                        }
                        catch(const std::out_of_range&) {
                            break ;
                        }

                        seq_idx ++ ;
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

        std::shared_ptr<util::Point2D> InputHinter::launch_loop(
                const std::vector<util::Point2D>& positions,
                const std::vector<Hint>& hints) {

            pimpl->init(positions.size()) ;

            core::InstantKeyAbsorber ika ;
            core::CharLogger lgr{
                KEYCODE_ESC,
                KEYCODE_BKSPACE
            };

            auto& igate = core::InputGate::get_instance() ;

            while(!(pimpl->cancel_running_)) {
                pimpl->bg_.update() ;

                core::KeyLog log{igate.pressed_list().data()} ;
                if(!CHAR_LOGGED(lgr.logging_state(log))) {
                    continue ;
                }

                if(lgr.latest().is_containing(KEYCODE_ESC)) {
                    igate.release_virtually(KEYCODE_ESC) ;
                    return nullptr ;
                }

                if(lgr.latest().is_containing(KEYCODE_BKSPACE)) {
                    igate.release_virtually(KEYCODE_BKSPACE) ;
                    if(lgr.size() == 1) {
                        return nullptr ;
                    }

                    lgr.remove_from_back(2) ;

                    pimpl->validate_if_match_with_hints(lgr, hints) ; //update matching list
                    continue ;
                }


                auto full_match_idx = pimpl->validate_if_match_with_hints(lgr, hints) ;
                if(full_match_idx >= 0) {
                    return std::make_shared<util::Point2D>(
                            positions[full_match_idx].x(),
                            positions[full_match_idx].y()) ;
                }

                if(drawable_hints_num() == 0) {
                    lgr.remove_from_back(1) ;
                }
            }
            return nullptr ;
        }

        std::shared_future<std::shared_ptr<util::Point2D>> InputHinter::launch_async_loop(
                const std::vector<util::Point2D>& positions,
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
            std::lock_guard<std::mutex> socoped_lock{pimpl->mtx_} ;
            return pimpl->matched_counts_ ;
        }

        const std::size_t& InputHinter::drawable_hints_num() const noexcept {
            std::lock_guard<std::mutex> socoped_lock{pimpl->mtx_} ;
            return pimpl->drawable_hints_num_ ;
        }
    }
}
