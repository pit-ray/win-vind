#include "bind/uia/easy_click.hpp"

#include <future>
#include <memory>

#include "bind/uia/display_hinter.hpp"
#include "bind/uia/ec_hints.hpp"
#include "bind/uia/input_hinter.hpp"
#include "bind/uia/point_2d.hpp"
#include "bind/uia/ui_scanner.hpp"
#include "io/mouse.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

#include "bind/uia/easy_click_core.hpp"

namespace vind
{
    //EasyClickLeft
    struct EasyClickLeft::Impl {
        EasyClickCore core_{} ;
    } ;
    EasyClickLeft::EasyClickLeft()
    : BindedFuncCreator("easy_click_left"),
      pimpl(std::make_unique<Impl>())
    {}
    EasyClickLeft::~EasyClickLeft() noexcept                 = default ;
    EasyClickLeft::EasyClickLeft(EasyClickLeft&&)            = default ;
    EasyClickLeft& EasyClickLeft::operator=(EasyClickLeft&&) = default ;

    void EasyClickLeft::sprocess() const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        pimpl->core_.scan_ui_objects(hwnd) ;
        pimpl->core_.create_matching_loop(KEYCODE_MOUSE_LEFT) ;
    }
    void EasyClickLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EasyClickLeft::load_config() {
        pimpl->core_.load_config() ;
    }


    //EasyClickRight
    struct EasyClickRight::Impl {
        EasyClickCore core_{} ;
    } ;
    EasyClickRight::EasyClickRight()
    : BindedFuncCreator("easy_click_right"),
      pimpl(std::make_unique<Impl>())
    {}
    EasyClickRight::~EasyClickRight() noexcept                  = default ;
    EasyClickRight::EasyClickRight(EasyClickRight&&)            = default ;
    EasyClickRight& EasyClickRight::operator=(EasyClickRight&&) = default ;

    void EasyClickRight::sprocess() const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        pimpl->core_.scan_ui_objects(hwnd) ;
        pimpl->core_.create_matching_loop(KEYCODE_MOUSE_RIGHT) ;
    }
    void EasyClickRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EasyClickRight::load_config() {
        pimpl->core_.load_config() ;
    }


    //EasyClickMid
    struct EasyClickMid::Impl {
        EasyClickCore core_{} ;
    } ;
    EasyClickMid::EasyClickMid()
    : BindedFuncCreator("easy_click_mid"),
      pimpl(std::make_unique<Impl>())
    {}
    EasyClickMid::~EasyClickMid() noexcept                = default ;
    EasyClickMid::EasyClickMid(EasyClickMid&&)            = default ;
    EasyClickMid& EasyClickMid::operator=(EasyClickMid&&) = default ;

    void EasyClickMid::sprocess() const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        pimpl->core_.scan_ui_objects(hwnd) ;
        pimpl->core_.create_matching_loop(KEYCODE_MOUSE_MID) ;
    }
    void EasyClickMid::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickMid::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EasyClickMid::load_config() {
        pimpl->core_.load_config() ;
    }


    //EasyClickHover
    struct EasyClickHover::Impl {
        EasyClickCore core_{} ;
    } ;
    EasyClickHover::EasyClickHover()
    : BindedFuncCreator("easy_click_hover"),
      pimpl(std::make_unique<Impl>())
    {}
    EasyClickHover::~EasyClickHover() noexcept                  = default ;
    EasyClickHover::EasyClickHover(EasyClickHover&&)            = default ;
    EasyClickHover& EasyClickHover::operator=(EasyClickHover&&) = default ;

    void EasyClickHover::sprocess() const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        pimpl->core_.scan_ui_objects(hwnd) ;
        pimpl->core_.create_matching_loop() ;
    }
    void EasyClickHover::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickHover::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EasyClickHover::load_config() {
        pimpl->core_.load_config() ;
    }
}
