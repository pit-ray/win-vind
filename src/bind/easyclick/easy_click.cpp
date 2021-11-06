#include "easy_click.hpp"

#include <future>
#include <memory>

#include "bind/easyclick/display_hinter.hpp"
#include "bind/easyclick/ec_hints.hpp"
#include "bind/easyclick/input_hinter.hpp"
#include "bind/easyclick/ui_scanner.hpp"
#include "core/keycode_def.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/mouse.hpp"
#include "util/winwrap.hpp"

#include "bind/easyclick/easy_click_core.hpp"

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

    void EasyClickLeft::sprocess(unsigned int repeat_num) const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        pimpl->core_.scan_ui_objects(hwnd) ;
        pimpl->core_.create_matching_loop(KEYCODE_MOUSE_LEFT, repeat_num) ;
    }
    void EasyClickLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EasyClickLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EasyClickLeft::reconstruct() {
        pimpl->core_.reconstruct() ;
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

    void EasyClickRight::sprocess(unsigned int repeat_num) const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        pimpl->core_.scan_ui_objects(hwnd) ;
        pimpl->core_.create_matching_loop(KEYCODE_MOUSE_RIGHT, repeat_num) ;
    }
    void EasyClickRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EasyClickRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EasyClickRight::reconstruct() {
        pimpl->core_.reconstruct() ;
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

    void EasyClickMid::sprocess(unsigned int repeat_num) const {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("Could not find a foreground window.") ;
        }
        pimpl->core_.scan_ui_objects(hwnd) ;
        pimpl->core_.create_matching_loop(KEYCODE_MOUSE_MID, repeat_num) ;
    }
    void EasyClickMid::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EasyClickMid::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void EasyClickMid::reconstruct() {
        pimpl->core_.reconstruct() ;
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
    void EasyClickHover::reconstruct() {
        pimpl->core_.reconstruct() ;
    }
}
