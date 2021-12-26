#include "movecursor.hpp"

#include <windows.h>

#include <chrono>
#include <cmath>

#include "core/errlogger.hpp"
#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "util/constant_accelerator.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/math.hpp"
#include "util/mouse.hpp"


namespace vind
{
    namespace bind
    {
        //MoveCursorLeft
        struct MoveCursorLeft::Impl {
            util::ConstAccelerator ca_{} ;
        } ;

        MoveCursorLeft::MoveCursorLeft()
        : BindedFuncVoid("move_cursor_left"),
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
            auto& settable = core::SetTable::get_instance() ;
            pimpl->ca_.set_acceleration(
                    settable.get("cursor_accel").get<float>()) ;
            pimpl->ca_.set_max_velocity(
                    settable.get("cursor_maxv").get<float>()) ;
            pimpl->ca_.set_time_weight(
                    settable.get("cursor_tweight").get<int>()) ;
        }


        //MoveCursorRight
        struct MoveCursorRight::Impl {
            util::ConstAccelerator ca_{} ;
        } ;

        MoveCursorRight::MoveCursorRight()
        : BindedFuncVoid("move_cursor_right"),
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
            auto& settable = core::SetTable::get_instance() ;
            pimpl->ca_.set_acceleration(
                    settable.get("cursor_accel").get<float>()) ;
            pimpl->ca_.set_max_velocity(
                    settable.get("cursor_maxv").get<float>()) ;
            pimpl->ca_.set_time_weight(
                    settable.get("cursor_tweight").get<int>()) ;
        }


        //MoveCursorUp
        struct MoveCursorUp::Impl {
            util::ConstAccelerator ca_{} ;
        } ;

        MoveCursorUp::MoveCursorUp()
        : BindedFuncVoid("move_cursor_up"),
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
            auto& settable = core::SetTable::get_instance() ;
            pimpl->ca_.set_acceleration(
                    settable.get("cursor_accel").get<float>()) ;
            pimpl->ca_.set_max_velocity(
                    settable.get("cursor_maxv").get<float>()) ;
            pimpl->ca_.set_time_weight(
                    settable.get("cursor_tweight").get<int>()) ;
        }


        //MoveCursorDown
        struct MoveCursorDown::Impl {
            util::ConstAccelerator ca_{} ;
        } ;

        MoveCursorDown::MoveCursorDown()
        : BindedFuncVoid("move_cursor_down"),
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
            auto& settable = core::SetTable::get_instance() ;
            pimpl->ca_.set_acceleration(
                    settable.get("cursor_accel").get<float>()) ;
            pimpl->ca_.set_max_velocity(
                    settable.get("cursor_maxv").get<float>()) ;
            pimpl->ca_.set_time_weight(
                    settable.get("cursor_tweight").get<int>()) ;
        }
    }
}
