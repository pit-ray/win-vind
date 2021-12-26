#include "opt/async_uia_cache_builder.hpp"

#include "core/inputgate.hpp"
#include "core/mode.hpp"
#include "core/settable.hpp"
#include "util/point_2d.hpp"
#include "util/uia.hpp"
#include "util/winwrap.hpp"

#include <chrono>
#include <unordered_map>

#if defined(DEBUG)
#include "util/debug.hpp"
#include <iostream>
#endif


namespace vind
{
    namespace opt
    {
        std::unordered_map<HWND, UICache> AsyncUIACacheBuilder::caches_{} ;

        AsyncUIACacheBuilder::AsyncUIACacheBuilder()
        : OptionCreator("uiacachebuild")
        {}

        void AsyncUIACacheBuilder::do_enable() const {
        }

        void AsyncUIACacheBuilder::do_disable() const {
            caches_.clear() ;
        }

        void AsyncUIACacheBuilder::do_process() const {
            // Ignore in Insert Mode or Resident Mode.
            using core::Mode ;
            auto mode = core::get_global_mode() ;
            if(mode == Mode::RESIDENT || mode == Mode::INSERT) {
                return ;
            }

            // Use some factors for minimal scanning

            // Factor1: keybaord input
            if(!core::InputGate::get_instance().pressed_list().empty()) {
                return ;
            }

            // Factor2: cursor position
            using namespace std::chrono ;
            static util::Point2D prepos ;
            static auto keeptime = system_clock::now() ;

            util::Point2D pos ;
            if(!GetCursorPos(&pos.data())) {
                throw RUNTIME_EXCEPT("Could not get the mouse cursor position.") ;
            }
            if(pos != prepos) {
                prepos = pos ;
                keeptime = system_clock::now() ;
                return ;
            }

            auto& settable = core::SetTable::get_instance() ;

            /**
             * NOTE: When the mouse is staying, scan only for the initial
             *       time range. When the mouse is moving, the cache is 
             *       less reliable, and when the computer is neglected 
             *       for a long time, there is no need to scan it.
             */
            auto keep_delta = duration_cast<milliseconds>(
                    system_clock::now() - keeptime).count() ;
            if(keep_delta < settable.get("uiacachebuild_staybegin").get<int>()) {
                return ;
            }
            if(keep_delta > settable.get("uiacachebuild_stayend").get<int>()) {
                return ;
            }

            auto hwnd = GetForegroundWindow() ;
            if(hwnd == NULL) { // don't scan for desktop root
                return ;
            }

            if(!has_cache(hwnd)) {
                caches_.emplace(hwnd, hwnd) ;
            }
            caches_[hwnd].update() ;
        }

        void AsyncUIACacheBuilder::register_property(PROPERTYID id) {
            add_common_cache_property(id) ;
        }

        util::SmartElement AsyncUIACacheBuilder::get_root_element(HWND hwnd) {
            if(!has_cache(hwnd)) {
                caches_.emplace(hwnd, hwnd) ;
            }
            return caches_[hwnd].latest() ;
        }
    }
}
