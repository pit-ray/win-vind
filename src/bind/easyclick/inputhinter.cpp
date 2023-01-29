#include "inputhinter.hpp"

#include <future>
#include <memory>
#include <mutex>
#include <vector>

#include "core/background.hpp"
#include "core/cmdmatcher.hpp"
#include "core/cmdunit.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/typeemu.hpp"
#include "opt/optionlist.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"


namespace vind
{
    namespace bind
    {
        struct InputHinter::Impl {
            std::vector<unsigned char> matched_counts_{} ;
            std::size_t drawable_hints_num_ = 0 ;
            std::atomic<bool> cancel_running_ = false ;
            std::mutex mtx_{} ;

            core::Background bg_ {
                opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::VCmdLine().name())
            } ;
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
            auto& igate = core::InputGate::get_instance() ;
            auto& ihub = core::InputHub::get_instance() ;

            pimpl->mtx_.lock() ;    // %%%%%% LOCK %%%%%%

            pimpl->matched_counts_.clear() ;
            pimpl->matched_counts_.resize(positions.size()) ;
            pimpl->drawable_hints_num_ = positions.size() ;
            pimpl->cancel_running_ = false ;

            pimpl->mtx_.unlock() ;  // %%%%%% UNLOCK %%%%%%

            std::vector<core::CmdMatcher> matchers{} ;
            for(const auto& hint : hints) {
                std::vector<core::CmdUnit::SPtr> cmds ;
                for(const auto& unit : hint) {
                    auto unit_ptr = std::make_shared<core::CmdUnit>(unit) ;
                    cmds.push_back(std::move(unit_ptr)) ;
                }
                matchers.emplace_back(std::move(cmds)) ;
            }

            core::InstantKeyAbsorber ika ;
            while(!(pimpl->cancel_running_)) {
                pimpl->bg_.update() ;

                core::CmdUnit::SPtr in_cmdunit ;
                std::uint16_t count ;
                if(!ihub.fetch_input(
                        in_cmdunit, count, core::get_global_mode(), false)) {
                    continue ;
                }

                std::lock_guard<std::mutex> socoped_lock{pimpl->mtx_} ;

                if(in_cmdunit->is_containing(KEYCODE_BKSPACE)) {
                    igate.release_virtually(KEYCODE_BKSPACE) ;
                    pimpl->drawable_hints_num_ = 0 ;
                    for(std::size_t i = 0 ; i < matchers.size() ; i ++) {
                        auto& mt = matchers[i] ;
                        mt.backward_state(1) ;

                        if(mt.is_rejected()) {
                            pimpl->matched_counts_[i] = 0 ;
                        }
                        else {
                            pimpl->matched_counts_[i] = \
                               static_cast<unsigned char>(mt.history_size()) ;
                            pimpl->drawable_hints_num_ ++ ;
                        }
                    }
                    continue ;
                }

                if(in_cmdunit->is_containing(KEYCODE_ESC)) {
                    break ;
                }

                // Fetch out only the characters.
                core::CmdUnitSet ascii_set{} ;
                for(auto& key : *in_cmdunit) {
                    if(!key.is_major_system()) {
                        ascii_set.insert(key) ;
                    }
                }
                if(ascii_set.empty()) {
                    continue ;
                }
                core::CmdUnit ascii_unit(std::move(ascii_set)) ;

                pimpl->drawable_hints_num_ = 0 ;
                bool all_rejected = true ;
                for(std::size_t i = 0 ; i < matchers.size() ; i ++) {
                    auto& mt = matchers[i] ;
                    mt.update_state(ascii_unit) ;
                    if(mt.is_accepted()) {
                        return std::make_shared<util::Point2D>(
                                positions[i].x(), positions[i].y()) ;
                    }

                    if(mt.is_rejected()) {
                        pimpl->matched_counts_[i] = 0 ;
                    }
                    else {
                        all_rejected = false ;
                        pimpl->matched_counts_[i] = \
                           static_cast<unsigned char>(mt.history_size()) ;
                        pimpl->drawable_hints_num_ ++ ;
                    }
                }
                if(all_rejected) {
                    break ;
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
