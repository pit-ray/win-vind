#include "bind/uia/easy_click.hpp"

#include <future>
#include <memory>

#include "bind/base/ntype_logger.hpp"
#include "bind/uia/display_hinter.hpp"
#include "bind/uia/ec_hints.hpp"
#include "bind/uia/input_hinter.hpp"
#include "bind/uia/point_2d.hpp"
#include "bind/uia/ui_scanner.hpp"
#include "io/mouse.hpp"
#include "key/keycode_def.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

namespace vind
{
    struct EasyClick::Impl {
        UIScanner scanner_{} ;
        std::vector<Point2D> positions_{} ;
        std::vector<Hint> hints_{} ;
        std::vector<std::string> strhints_{} ;
        InputHinter input_hinter_{} ;
        DisplayHinter display_hinter_{} ;
    } ;

    EasyClick::EasyClick()
    : pimpl(std::make_unique<Impl>())
    {
        pimpl->positions_.reserve(4096) ;
        pimpl->hints_.reserve(2048) ;
        pimpl->strhints_.reserve(2048) ;
    }

    EasyClick::~EasyClick() noexcept             = default ;
    EasyClick::EasyClick(EasyClick&&)            = default ;
    EasyClick& EasyClick::operator=(EasyClick&&) = default ;

    void EasyClick::scan_ui_objects(HWND hwnd) const {
        pimpl->scanner_.scan(hwnd, pimpl->positions_, true) ;
        assign_identifier_hints(pimpl->positions_.size(), pimpl->hints_) ;
        convert_hints_to_strings(pimpl->hints_, pimpl->strhints_) ;
    }

    void EasyClick::create_matching_loop(KeyCode sendkey) const {

        auto ft = pimpl->input_hinter_.launch_async_loop(
                pimpl->positions_,
                pimpl->hints_) ;

        using namespace std::chrono ;
        while(ft.wait_for(50ms) == std::future_status::timeout) {
            try {
                if(pimpl->input_hinter_.must_draw_hints_num() == pimpl->hints_.size()) {
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
    }


    //EasyClickLeft
    EasyClickLeft::EasyClickLeft()
    : BindedFuncCreator("easy_click_left"),
      EasyClick()
    {}
    void EasyClickLeft::sprocess() const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        scan_ui_objects(hwnd) ;
        create_matching_loop(KEYCODE_MOUSE_LEFT) ;
    }
    void EasyClickLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //EasyClickRight
    EasyClickRight::EasyClickRight()
    : BindedFuncCreator("easy_click_right"),
      EasyClick()
    {}
    void EasyClickRight::sprocess() const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        scan_ui_objects(hwnd) ;
        create_matching_loop(KEYCODE_MOUSE_RIGHT) ;
    }
    void EasyClickRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //EasyClickMid
    EasyClickMid::EasyClickMid()
    : BindedFuncCreator("easy_click_mid"),
      EasyClick()
    {}
    void EasyClickMid::sprocess() const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        scan_ui_objects(hwnd) ;
        create_matching_loop(KEYCODE_MOUSE_MID) ;
    }
    void EasyClickMid::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickMid::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //EasyClickHover
    EasyClickHover::EasyClickHover()
    : BindedFuncCreator("easy_click_hover"),
      EasyClick()
    {}
    void EasyClickHover::sprocess() const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        scan_ui_objects(hwnd) ;
        create_matching_loop() ;
    }
    void EasyClickHover::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickHover::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
}
