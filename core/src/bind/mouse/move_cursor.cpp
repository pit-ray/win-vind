#include "bind/mouse/move_cursor.hpp"

#include <windows.h>

#include <chrono>
#include <cmath>
#include <iostream>

#include "err_logger.hpp"
#include "g_params.hpp"
#include "io/mouse.hpp"
#include "key/ntype_logger.hpp"
#include "time/constant_accelerator.hpp"
#include "util/def.hpp"
#include "util/math.hpp"


namespace vind
{
    //MoveCursorLeft
    struct MoveCursorLeft::Impl {
        ConstAccelerator ca_{} ;
    } ;

    MoveCursorLeft::MoveCursorLeft()
    : BindedFuncCreator("move_cursor_left"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorLeft::~MoveCursorLeft() noexcept                  = default ;
    MoveCursorLeft::MoveCursorLeft(MoveCursorLeft&&)            = default ;
    MoveCursorLeft& MoveCursorLeft::operator=(MoveCursorLeft&&) = default ;

    void MoveCursorLeft::sprocess(int delta) const {
        mouse::move_cursor(-delta, 0) ;
    }
    void MoveCursorLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->ca_.reset() ;
        }
        sprocess(pimpl->ca_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void MoveCursorLeft::reconstruct() {
        pimpl->ca_.set_acceleration(gparams::get_f("cursor_acceleration")) ;
        pimpl->ca_.set_max_velocity(gparams::get_f("cursor_max_velocity")) ;
        pimpl->ca_.set_time_weight(gparams::get_i("cursor_time_weight")) ;
    }


    //MoveCursorRight
    struct MoveCursorRight::Impl {
        ConstAccelerator ca_{} ;
    } ;

    MoveCursorRight::MoveCursorRight()
    : BindedFuncCreator("move_cursor_right"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorRight::~MoveCursorRight() noexcept                   = default ;
    MoveCursorRight::MoveCursorRight(MoveCursorRight&&)            = default ;
    MoveCursorRight& MoveCursorRight::operator=(MoveCursorRight&&) = default ;

    void MoveCursorRight::sprocess(int delta) const {
        mouse::move_cursor(delta, 0) ;
    }
    void MoveCursorRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->ca_.reset() ;
        }
        sprocess(pimpl->ca_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void MoveCursorRight::reconstruct() {
        pimpl->ca_.set_acceleration(gparams::get_f("cursor_acceleration")) ;
        pimpl->ca_.set_max_velocity(gparams::get_f("cursor_max_velocity")) ;
        pimpl->ca_.set_time_weight(gparams::get_i("cursor_time_weight")) ;
    }


    //MoveCursorUp
    struct MoveCursorUp::Impl {
        ConstAccelerator ca_{} ;
    } ;

    MoveCursorUp::MoveCursorUp()
    : BindedFuncCreator("move_cursor_up"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorUp::~MoveCursorUp() noexcept                = default ;
    MoveCursorUp::MoveCursorUp(MoveCursorUp&&)            = default ;
    MoveCursorUp& MoveCursorUp::operator=(MoveCursorUp&&) = default ;

    void MoveCursorUp::sprocess(int delta) const {
        mouse::move_cursor(0, -delta) ;
    }
    void MoveCursorUp::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->ca_.reset() ;
        }
        sprocess(pimpl->ca_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorUp::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void MoveCursorUp::reconstruct() {
        pimpl->ca_.set_acceleration(gparams::get_f("cursor_acceleration")) ;
        pimpl->ca_.set_max_velocity(gparams::get_f("cursor_max_velocity")) ;
        pimpl->ca_.set_time_weight(gparams::get_i("cursor_time_weight")) ;
    }


    //MoveCursorDown
    struct MoveCursorDown::Impl {
        ConstAccelerator ca_{} ;
    } ;

    MoveCursorDown::MoveCursorDown()
    : BindedFuncCreator("move_cursor_down"),
      pimpl(std::make_unique<Impl>())
    {}

    MoveCursorDown::~MoveCursorDown() noexcept                  = default ;
    MoveCursorDown::MoveCursorDown(MoveCursorDown&&)            = default ;
    MoveCursorDown& MoveCursorDown::operator=(MoveCursorDown&&) = default ;

    void MoveCursorDown::sprocess(int delta) const {
        mouse::move_cursor(0, delta) ;
    }
    void MoveCursorDown::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            pimpl->ca_.reset() ;
        }
        sprocess(pimpl->ca_.delta<int>() * parent_lgr.get_head_num()) ;
    }
    void MoveCursorDown::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
    void MoveCursorDown::reconstruct() {
        pimpl->ca_.set_acceleration(gparams::get_f("cursor_acceleration")) ;
        pimpl->ca_.set_max_velocity(gparams::get_f("cursor_max_velocity")) ;
        pimpl->ca_.set_time_weight(gparams::get_i("cursor_time_weight")) ;
    }
}
