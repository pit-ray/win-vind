#include "move_cursor.hpp"

#include <windows.h>

#include <chrono>
#include <cmath>

#include "core/err_logger.hpp"
#include "core/g_params.hpp"
#include "core/ntype_logger.hpp"
#include "util/constant_accelerator.hpp"
#include "util/def.hpp"
#include "util/math.hpp"
#include "util/mouse.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    //MoveCursorLeft
    struct MoveCursorLeft::Impl {
        util::ConstAccelerator ca_{} ;
    } ;

    MoveCursorLeft::MoveCursorLeft()
    : BindedFuncCreator("move_cursor_left"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorLeft::~MoveCursorLeft() noexcept                  = default ;
    MoveCursorLeft::MoveCursorLeft(MoveCursorLeft&&)            = default ;
    MoveCursorLeft& MoveCursorLeft::operator=(MoveCursorLeft&&) = default ;

    void MoveCursorLeft::sprocess(int delta) const {
        util::move_cursor(-delta, 0) ;
    }
    void MoveCursorLeft::sprocess(core::NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->ca_.reset() ;
        }
        sprocess(pimpl->ca_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorLeft::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void MoveCursorLeft::reconstruct() {
        pimpl->ca_.set_acceleration(core::get_f("cursor_accel")) ;
        pimpl->ca_.set_max_velocity(core::get_f("cursor_maxv")) ;
        pimpl->ca_.set_time_weight(core::get_i("cursor_tweight")) ;
    }


    //MoveCursorRight
    struct MoveCursorRight::Impl {
        util::ConstAccelerator ca_{} ;
    } ;

    MoveCursorRight::MoveCursorRight()
    : BindedFuncCreator("move_cursor_right"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorRight::~MoveCursorRight() noexcept                   = default ;
    MoveCursorRight::MoveCursorRight(MoveCursorRight&&)            = default ;
    MoveCursorRight& MoveCursorRight::operator=(MoveCursorRight&&) = default ;

    void MoveCursorRight::sprocess(int delta) const {
        util::move_cursor(delta, 0) ;
    }
    void MoveCursorRight::sprocess(core::NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->ca_.reset() ;
        }
        sprocess(pimpl->ca_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorRight::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void MoveCursorRight::reconstruct() {
        pimpl->ca_.set_acceleration(core::get_f("cursor_accel")) ;
        pimpl->ca_.set_max_velocity(core::get_f("cursor_maxv")) ;
        pimpl->ca_.set_time_weight(core::get_i("cursor_tweight")) ;
    }


    //MoveCursorUp
    struct MoveCursorUp::Impl {
        util::ConstAccelerator ca_{} ;
    } ;

    MoveCursorUp::MoveCursorUp()
    : BindedFuncCreator("move_cursor_up"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorUp::~MoveCursorUp() noexcept                = default ;
    MoveCursorUp::MoveCursorUp(MoveCursorUp&&)            = default ;
    MoveCursorUp& MoveCursorUp::operator=(MoveCursorUp&&) = default ;

    void MoveCursorUp::sprocess(int delta) const {
        util::move_cursor(0, -delta) ;
    }
    void MoveCursorUp::sprocess(core::NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->ca_.reset() ;
        }
        sprocess(pimpl->ca_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorUp::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void MoveCursorUp::reconstruct() {
        pimpl->ca_.set_acceleration(core::get_f("cursor_accel")) ;
        pimpl->ca_.set_max_velocity(core::get_f("cursor_maxv")) ;
        pimpl->ca_.set_time_weight(core::get_i("cursor_tweight")) ;
    }


    //MoveCursorDown
    struct MoveCursorDown::Impl {
        util::ConstAccelerator ca_{} ;
    } ;

    MoveCursorDown::MoveCursorDown()
    : BindedFuncCreator("move_cursor_down"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorDown::~MoveCursorDown() noexcept                  = default ;
    MoveCursorDown::MoveCursorDown(MoveCursorDown&&)            = default ;
    MoveCursorDown& MoveCursorDown::operator=(MoveCursorDown&&) = default ;

    void MoveCursorDown::sprocess(int delta) const {
        util::move_cursor(0, delta) ;
    }
    void MoveCursorDown::sprocess(core::NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->ca_.reset() ;
        }
        sprocess(pimpl->ca_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorDown::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void MoveCursorDown::reconstruct() {
        pimpl->ca_.set_acceleration(core::get_f("cursor_accel")) ;
        pimpl->ca_.set_max_velocity(core::get_f("cursor_maxv")) ;
        pimpl->ca_.set_time_weight(core::get_i("cursor_tweight")) ;
    }
}
