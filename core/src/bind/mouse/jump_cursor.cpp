#include "bind/mouse/jump_cursor.hpp"

#include "g_params.hpp"
#include "io/screen_metrics.hpp"

#include "util/def.hpp"

#include "key/ntype_logger.hpp"

namespace vind
{
    //JumpToLeft
    JumpToLeft::JumpToLeft()
    : BindedFuncCreator("jump_to_left")
    {}
    void JumpToLeft::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;
        SetCursorPos(0, pos.y) ;
    }
    void JumpToLeft::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void JumpToLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //JumpToRight
    JumpToRight::JumpToRight()
    : BindedFuncCreator("jump_to_right")
    {}
    void JumpToRight::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;

        SetCursorPos(screenmetrics::width(rect) - gparams::get_i("jump_margin"), pos.y) ;
    }
    void JumpToRight::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void JumpToRight::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //JumpToTop
    JumpToTop::JumpToTop()
    : BindedFuncCreator("jump_to_top")
    {}
    void JumpToTop::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;

        SetCursorPos(pos.x, rect.top) ;
    }
    void JumpToTop::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void JumpToTop::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //JumpToBottom
    JumpToBottom::JumpToBottom()
    : BindedFuncCreator("jump_to_bottom")
    {}
    void JumpToBottom::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;
        SetCursorPos(pos.x, screenmetrics::height(rect) - gparams::get_i("jump_margin")) ;
    }
    void JumpToBottom::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void JumpToBottom::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //JumpToHorizontalCenter
    JumpToHorizontalCenter::JumpToHorizontalCenter()
    : BindedFuncCreator("jump_to_hcenter")
    {}
    void JumpToHorizontalCenter::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;
        SetCursorPos(screenmetrics::width(rect) / 2, pos.y) ;
    }
    void JumpToHorizontalCenter::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void JumpToHorizontalCenter::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //JumpToVerticalCenter
    JumpToVerticalCenter::JumpToVerticalCenter()
    : BindedFuncCreator("jump_to_vcenter")
    {}
    void JumpToVerticalCenter::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;
        SetCursorPos(pos.x, screenmetrics::height(rect) / 2) ;
    }
    void JumpToVerticalCenter::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void JumpToVerticalCenter::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
