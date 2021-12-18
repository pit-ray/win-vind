#include "jump_cursor.hpp"

#include "core/ntype_logger.hpp"
#include "core/settable.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/screen_metrics.hpp"

namespace vind
{
    namespace bind
    {
        //JumpToLeft
        JumpToLeft::JumpToLeft()
        : BindedFuncVoid("jump_cursor_to_left")
        {}
        void JumpToLeft::sprocess() {
            POINT pos ;
            GetCursorPos(&pos) ;
            SetCursorPos(0, pos.y) ;
        }
        void JumpToLeft::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void JumpToLeft::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //JumpToRight
        JumpToRight::JumpToRight()
        : BindedFuncVoid("jump_cursor_to_right")
        {}
        void JumpToRight::sprocess() {
            POINT pos ;
            GetCursorPos(&pos) ;

            auto box = util::get_conbined_metrics() ;

            auto& settable = core::SetTable::get_instance() ;
            SetCursorPos(
                    box.width() - settable.get("jump_margin").get<int>(),
                    pos.y) ;
        }
        void JumpToRight::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void JumpToRight::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //JumpToTop
        JumpToTop::JumpToTop()
        : BindedFuncVoid("jump_cursor_to_top")
        {}
        void JumpToTop::sprocess() {
            POINT pos ;
            GetCursorPos(&pos) ;

            auto box = util::get_conbined_metrics() ;

            SetCursorPos(pos.x, box.top()) ;
        }
        void JumpToTop::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void JumpToTop::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //JumpToBottom
        JumpToBottom::JumpToBottom()
        : BindedFuncVoid("jump_cursor_to_bottom")
        {}
        void JumpToBottom::sprocess() {
            POINT pos ;
            GetCursorPos(&pos) ;

            auto box = util::get_conbined_metrics() ;

            auto& settable = core::SetTable::get_instance() ;
            SetCursorPos(
                    pos.x,
                    box.height() - settable.get("jump_margin").get<int>()) ;
        }
        void JumpToBottom::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void JumpToBottom::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //JumpToHorizontalCenter
        JumpToHorizontalCenter::JumpToHorizontalCenter()
        : BindedFuncVoid("jump_cursor_to_hcenter")
        {}
        void JumpToHorizontalCenter::sprocess() {
            POINT pos ;
            GetCursorPos(&pos) ;

            auto box = util::get_conbined_metrics() ;
            SetCursorPos(box.center_x(), pos.y) ;
        }
        void JumpToHorizontalCenter::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void JumpToHorizontalCenter::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //JumpToVerticalCenter
        JumpToVerticalCenter::JumpToVerticalCenter()
        : BindedFuncVoid("jump_cursor_to_vcenter")
        {}
        void JumpToVerticalCenter::sprocess() {
            POINT pos ;
            GetCursorPos(&pos) ;

            auto box = util::get_conbined_metrics() ;
            SetCursorPos(pos.x, box.center_y()) ;
        }
        void JumpToVerticalCenter::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void JumpToVerticalCenter::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
