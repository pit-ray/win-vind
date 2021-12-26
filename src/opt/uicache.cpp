#include "uicache.hpp"

#include "core/settable.hpp"
#include "util/uia.hpp"

#include <chrono>
#include <future>


namespace vind
{
    namespace opt
    {
        class TimeStamp {
        private:
            std::chrono::system_clock::time_point begin_{} ;
            std::chrono::system_clock::time_point end_{} ;
            std::chrono::milliseconds mean_{0} ;

        public:
            void stamp_begin() noexcept {
                begin_ = std::chrono::system_clock::now() ;
            }
            void stamp_end() noexcept {
                end_ = std::chrono::system_clock::now() ;
            }

            bool has_stamp() const noexcept {
                return end_.time_since_epoch().count() != 0 ;
            }

            auto elapsed() noexcept {
                using namespace std::chrono ;
                return duration_cast<milliseconds>(system_clock::now() - begin_) ;
            }

            auto is_elapsed_expect() noexcept {
                using namespace std::chrono ;
                return (elapsed() - mean_) >= 0ms ;
            }

            void update_average() noexcept {
                using namespace std::chrono ;
                auto d = duration_cast<milliseconds>(end_ - begin_) ;
                if(mean_ == 0ms) {
                    mean_ = d ;
                }
                else {
                    // Dynamically calculate the average elapsed time
                    // with exponential moving average (approximation).
                    mean_ = (mean_ + d) / 2 ;
                }
            }
        } ;


        auto& shared_cache_request() {
            static auto instance = [] {
                auto req = util::create_cache_request() ;

                util::add_property(req, UIA_IsEnabledPropertyId) ;
                util::add_property(req, UIA_IsOffscreenPropertyId) ;
                util::add_property(req, UIA_BoundingRectanglePropertyId) ;

                util::switch_mode(req, true) ; // enable full control
                util::change_scope(req, TreeScope::TreeScope_Subtree) ;

                return req ;
            }() ;

            return instance ;
        }

        void add_common_cache_property(PROPERTYID id) {
            util::add_property(shared_cache_request(), id) ;
        }


        struct UICache::Impl {
            util::SmartElement root_ ;
            std::future<util::SmartElement> ft_ ;
            TimeStamp time_ ;

            Impl()
            : root_(nullptr),
              ft_(),
              time_()
            {}

            template <typename UIElem>
            Impl(UIElem&& elem)
            : root_(std::forward<UIElem>(elem)),
              ft_(),
              time_()
            {}
        } ;

        UICache::UICache()
        : pimpl(std::make_unique<Impl>())
        {}

        UICache::UICache(HWND hwnd)
        : pimpl(std::make_unique<Impl>(util::get_root_element(hwnd)))
        {}

        UICache::~UICache() noexcept = default ;
        UICache::UICache(UICache&&) = default ;
        UICache& UICache::operator=(UICache&&) = default ;

        bool UICache::is_valid() {
            if(!pimpl->root_) {
                return false ;
            }
            if(!pimpl->time_.has_stamp()) {
                return false ;
            }

            auto& settable = core::SetTable::get_instance() ;
            return pimpl->time_.elapsed().count() \
                < settable.get("uiacachebuild_lifetime").get<int>() ;
        }

        void UICache::create_thread() {
            if(!pimpl->root_) {
                return ;
            }

            pimpl->ft_ = std::async(std::launch::async, [this] {
                pimpl->time_.stamp_begin() ;

                auto updated = util::update_element(pimpl->root_, shared_cache_request()) ;

                pimpl->time_.stamp_end() ;
                pimpl->time_.update_average() ;
                return updated ;
            }) ;
        }

        bool UICache::check_if_finished() {
            using namespace std::chrono ;
            return pimpl->ft_.wait_for(1us) != std::future_status::timeout ;
        }

        void UICache::update() {
            if(is_valid() || !pimpl->root_) {
                return ;
            }

            // Since checking the shared state is time consuming,
            // optimize intervals during running to minimize checking.
            if(!pimpl->time_.is_elapsed_expect()) {
                return ;
            }

            if(pimpl->ft_.valid()) {
                // This checks the shared state,
                // but it takes several tens of milliseconds to see the shared state.
                if(check_if_finished()) {
                    pimpl->root_ = pimpl->ft_.get() ;
                }

                // If you stack threads in the array even though
                // the process to build a cache does not finish in the valid period,
                // it may have an infinite number of threads.
                return ;
            }

            create_thread() ;
        }

        const util::SmartElement& UICache::latest() {
            if(!pimpl->root_) {
                // If have no initialized SmartElement, return nullptr.
                return pimpl->root_ ;
            }

            if(is_valid()) {
                return pimpl->root_ ;
            }

            if(pimpl->ft_.valid()) {
                if(check_if_finished()) {
                    pimpl->root_ = pimpl->ft_.get() ;
                    return pimpl->root_ ;
                }
            }
            else {
                create_thread() ;
            }

            pimpl->ft_.wait() ;
            pimpl->root_ = pimpl->ft_.get() ;
            return pimpl->root_ ;
        }
    }
}
