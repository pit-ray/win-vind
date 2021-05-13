#include "bind/uia/easy_click_core.hpp"

#include <future>
#include <memory>

#include "bind/uia/display_hinter.hpp"
#include "bind/uia/ec_hints.hpp"
#include "bind/uia/input_hinter.hpp"
#include "bind/uia/point_2d.hpp"
#include "bind/uia/ui_scanner.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

namespace vind
{
    struct EasyClickCore::Impl {
        UIScanner scanner_{} ;
        std::vector<Point2D> positions_{} ;
        std::vector<Hint> hints_{} ;
        std::vector<std::string> strhints_{} ;
        InputHinter input_hinter_{} ;
        DisplayHinter display_hinter_{} ;
    } ;

    EasyClickCore::EasyClickCore()
    : pimpl(std::make_unique<Impl>())
    {
        pimpl->positions_.reserve(2048) ;
        pimpl->hints_.reserve(2048) ;
        pimpl->strhints_.reserve(2048) ;
    }

    EasyClickCore::~EasyClickCore() noexcept = default ;

    EasyClickCore::EasyClickCore(EasyClickCore&&)            = default ;
    EasyClickCore& EasyClickCore::operator=(EasyClickCore&&) = default ;

    void EasyClickCore::scan_ui_objects(HWND hwnd) const {
        pimpl->hints_.clear() ;
        pimpl->positions_.clear() ;
        pimpl->strhints_.clear() ;

        pimpl->scanner_.scan(hwnd, pimpl->positions_, true) ;
        assign_identifier_hints(pimpl->positions_.size(), pimpl->hints_) ;
        convert_hints_to_strings(pimpl->hints_, pimpl->strhints_) ;
    }

    void EasyClickCore::create_matching_loop(KeyCode sendkey) const {
        auto ft = pimpl->input_hinter_.launch_async_loop(
                pimpl->positions_,
                pimpl->hints_) ;

        using namespace std::chrono ;
        while(ft.wait_for(50ms) == std::future_status::timeout) {
            try {
                if(pimpl->input_hinter_.drawable_hints_num() == pimpl->hints_.size()) {
                    // Hints were not matched yet, so must draw all hints.
                    pimpl->display_hinter_.paint_all_hints(
                            pimpl->positions_, pimpl->strhints_) ;
                }
                else {
                    pimpl->display_hinter_.paint_matching_hints(
                            pimpl->positions_, pimpl->strhints_, pimpl->input_hinter_.matched_counts()) ;
                }
            }
            catch(const std::exception& e) {
                pimpl->input_hinter_.cancel() ;
                throw e ;
            }
        }

        util::refresh_display(NULL) ;

        if(auto pos = ft.get()) {
            SetCursorPos(pos->x(), pos->y()) ;
            mouse::click(sendkey) ;
        }

        //Release all keys in order to avoid the next matching right after.
        for(KeyCode key : keyabsorber::get_pressed_list()) {
            keyabsorber::release_virtually(key) ;
        }
    }

    void EasyClickCore::load_config() {
        pimpl->display_hinter_.load_config() ;
    }
}
