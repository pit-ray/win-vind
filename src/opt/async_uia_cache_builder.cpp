#include "opt/async_uia_cache_builder.hpp"

#include "core/g_params.hpp"
#include "core/key_absorber.hpp"
#include "core/mode.hpp"
#include "util/debug.hpp"
#include "util/point_2d.hpp"
#include "util/uia.hpp"
#include "util/winwrap.hpp"

#include <chrono>
#include <future>
#include <unordered_map>

#if defined(DEBUG)
#include <iostream>
#endif


namespace
{
    using namespace vind ;

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
                // Dynamically calculate the average elapsed time (approximation)
                mean_ = (mean_ + d) / 2 ;
            }
        }
    } ;

    auto g_cache_request = [] {
        auto req = uiauto::create_cache_request() ;

        uiauto::add_property(req, UIA_IsEnabledPropertyId) ;
        uiauto::add_property(req, UIA_IsOffscreenPropertyId) ;
        uiauto::add_property(req, UIA_BoundingRectanglePropertyId) ;

        uiauto::switch_mode(req, true) ; // enable full control
        uiauto::change_scope(req, TreeScope::TreeScope_Subtree) ;

        return req ;
    }() ;

    class WindowUICache {
    private:
        SmartElement root_{nullptr} ;
        std::future<SmartElement> ft_{} ;
        TimeStamp time_{} ;

    public:
        void initialize(HWND hwnd) {
            root_ = uiauto::get_root_element(hwnd) ;
        }

        bool is_valid() {
            if(!time_.has_stamp()) return false ;
            return time_.elapsed().count() \
                < gparams::get_i("uiacachebuild_lifetime") ;
        }

        void create_thread() {
            ft_ = std::async(std::launch::async, [this] {
                time_.stamp_begin() ;

                auto updated = uiauto::update_element(root_, g_cache_request) ;

                time_.stamp_end() ;
                time_.update_average() ;
                return updated ;
            }) ;
        }

        bool check_if_finished() {
            using namespace std::chrono ;
            return ft_.wait_for(1us) != std::future_status::timeout ;
        }

        void update() {
            if(is_valid() || !root_) {
                return ;
            }

            // Since checking the shared state is time consuming,
            // optimize intervals during running to minimize checking.
            if(!time_.is_elapsed_expect()) {
                return ;
            }

            if(ft_.valid()) {
                // This checks the shared state,
                // but it takes several tens of milliseconds to see the shared state.
                if(check_if_finished()) {
                    root_ = ft_.get() ;
                }

                // If you stack threads in the array even though
                // the process to build a cache does not finish in the valid period,
                // it may have an infinite number of threads.
                return ;
            }

            create_thread() ;
        }

        const SmartElement& latest() {
            if(is_valid()) {
                return root_ ;
            }

            if(ft_.valid()) {
                if(check_if_finished()) {
                    root_ = ft_.get() ;
                    return root_ ;
                }
            }
            else {
                create_thread() ;
            }

            ft_.wait() ;
            root_ = ft_.get() ;
            return root_ ;
        }
    } ;

    std::unordered_map<HWND, WindowUICache> g_caches{} ;
    inline auto has_cache(HWND hwnd) noexcept {
        return g_caches.find(hwnd) != g_caches.end() ;
    }
}


namespace vind
{
    AsyncUIACacheBuilder::AsyncUIACacheBuilder()
    : OptionCreator("uiacachebuild")
    {}

    void AsyncUIACacheBuilder::do_enable() const {
    }

    void AsyncUIACacheBuilder::do_disable() const {
        g_caches.clear() ;
    }

    void AsyncUIACacheBuilder::do_process() const {
        // Ignore in Inser Mode and Resident Mode.
        if(mode::get_global_mode() == mode::Mode::RESIDENT) {
            return ;
        }

        // Use some factors for minimal scanning

        // Factor1: keybaord input
        if(!keyabsorber::get_pressed_list().empty()) {
            return ;
        }

        // Factor2: cursor position
        using namespace std::chrono ;
        static Point2D prepos ;
        static auto keeptime = system_clock::now() ;

        Point2D pos ;
        if(!GetCursorPos(&pos.data())) {
            throw RUNTIME_EXCEPT("Could not get the mouse cursor position.") ;
        }
        if(pos != prepos) {
            prepos = pos ;
            keeptime = system_clock::now() ;
            return ;
        }

        // When the mouse is staying, scan only for the initial time range.
        // When the mouse is moving, the cache is less reliable,
        // and when the computer is neglected for a long time, there is no need to scan it.
        auto keep_delta = duration_cast<milliseconds>(
                system_clock::now() - keeptime).count() ;
        if(keep_delta < gparams::get_i("uiacachebuild_staybegin")) {
            return ;
        }
        if(keep_delta > gparams::get_i("uiacachebuild_stayend")) {
            return ;
        }

        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) { // don't scan for desktop root
            return ;
        }

        if(!has_cache(hwnd)) {
            g_caches[hwnd].initialize(hwnd) ;
        }
        g_caches[hwnd].update() ;
    }

    void AsyncUIACacheBuilder::register_property(PROPERTYID id) {
        uiauto::add_property(g_cache_request, id) ;
    }

    SmartElement AsyncUIACacheBuilder::get_root_element(HWND hwnd) {
        if(!has_cache(hwnd)) {
            g_caches[hwnd].initialize(hwnd) ;
        }
        return g_caches[hwnd].latest() ;
    }
}
