#include "opt/async_uia_cache_builder.hpp"

#include "g_params.hpp"
#include "key/key_absorber.hpp"
#include "uia/uia.hpp"
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

    struct WindowUICache {
        SmartElement root_{nullptr} ;
        std::chrono::system_clock::time_point time_{} ;
        std::future<SmartElement> ft_{} ;

        bool is_valid() {
            using namespace std::chrono ;
            auto now = system_clock::now() ;
            return duration_cast<milliseconds>(
                    now - time_).count() < gparams::get_i("uiacachebuild_validms") ;
        }

        void stamp_time() {
            time_ = std::chrono::system_clock::now() ;
        }
    } ;

    SmartCacheReq g_cache_request = [] {
        auto req = uiauto::create_cache_request() ;

        uiauto::add_property(req, UIA_IsEnabledPropertyId) ;
        uiauto::add_property(req, UIA_IsOffscreenPropertyId) ;
        uiauto::add_property(req, UIA_BoundingRectanglePropertyId) ;

        uiauto::switch_mode(req, true) ; // enable full control
        uiauto::change_scope(req, TreeScope::TreeScope_Subtree) ;

        return req ;
    }() ;

    std::unordered_map<HWND, WindowUICache> g_caches{} ;

    void update_cache(HWND hwnd) {
        auto& cache = g_caches[hwnd] ;

        if(cache.ft_.valid()) {
            using namespace std::chrono ;
            if(cache.ft_.wait_for(1ms) != std::future_status::timeout) {
                // build is finished
                cache.root_ = cache.ft_.get() ;
            }

            // If you stack threads in the array even though
            // the process to build a cache does not finish in the valid period,
            // it may have an infinite number of threads.
            return ;
        }

        if(!cache.is_valid()) {
            std::cout << "- expired\n" ;
            auto elem = uiauto::get_root_element(hwnd) ;

            cache.ft_ = std::async(std::launch::async, [elem, &cache] {
                auto updated = uiauto::update_element(elem, g_cache_request) ;
                cache.stamp_time() ;
                std::cout << "+ updated\n" ;
                return updated ;
            }) ;
        }
    }

    const SmartElement& get_latest_cache(HWND hwnd) {
        auto& cache = g_caches[hwnd] ;

        if(!cache.root_) {
            update_cache(hwnd) ;
            cache.ft_.wait() ;
            cache.root_ = cache.ft_.get() ;
            return cache.root_ ;
        }


        if(cache.ft_.valid()) {
            using namespace std::chrono ;
            if(cache.ft_.wait_for(1ms) != std::future_status::timeout) {
                // finished build
                cache.root_ = cache.ft_.get() ;
                return cache.root_ ;
            }

            // the process does not finish to build cache
            if(!cache.is_valid()) {
                cache.ft_.wait() ;
                cache.root_ = cache.ft_.get() ;
                return cache.root_ ;
            }

            return cache.root_ ;
        }

        if(!cache.is_valid()) {
            update_cache(hwnd) ;
            cache.ft_.wait() ;
            cache.root_ = cache.ft_.get() ;
            return cache.root_ ;
        }

        return cache.root_ ;
    }
}


namespace vind
{
    AsyncUIACacheBuilder::AsyncUIACacheBuilder()
    : OptionCreator("uiacachebuild")
    {}

    void AsyncUIACacheBuilder::do_enable() const {
        std::cout << "enabled\n" ;
    }

    void AsyncUIACacheBuilder::do_disable() const {
        g_caches.clear() ;
    }

    void AsyncUIACacheBuilder::do_process() const {
        if(!keyabsorber::is_absorbed()) {
            return ;
        }

        // detect window if move cursor or select new hwnd
        // Scan use std::async
        //
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) { // don't scan for desktop root
            return ;
        }

        update_cache(hwnd) ;
    }

    void AsyncUIACacheBuilder::register_property(PROPERTYID id) {
        uiauto::add_property(g_cache_request, id) ;
    }

    SmartElement AsyncUIACacheBuilder::get_root_element(HWND hwnd) {
        return get_latest_cache(hwnd) ;
    }
}
