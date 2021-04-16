#include "bind/mouse/jump_cursor.hpp"

#include "io/screen_metrics.hpp"
#include "coreio/i_params.hpp"

#include "util/def.hpp"

#include "bind/base/ntype_logger.hpp"

namespace vind
{
    //Jump2Left
    Jump2Left::Jump2Left()
    : BindedFuncCreator("jump_to_left")
    {}
    void Jump2Left::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;
        SetCursorPos(0, pos.y) ;
    }
    void Jump2Left::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Jump2Left::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //Jump2Right
    Jump2Right::Jump2Right()
    : BindedFuncCreator("jump_to_right")
    {}
    void Jump2Right::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;

        SetCursorPos(screenmetrics::width(rect) - iparams::get_i("screen_pos_buf"), pos.y) ;
    }
    void Jump2Right::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Jump2Right::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //Jump2Top
    Jump2Top::Jump2Top()
    : BindedFuncCreator("jump_to_top")
    {}
    void Jump2Top::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;
        SetCursorPos(pos.x, 0) ;
    }
    void Jump2Top::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Jump2Top::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //Jump2Bottom
    Jump2Bottom::Jump2Bottom()
    : BindedFuncCreator("jump_to_bottom")
    {}
    void Jump2Bottom::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;
        SetCursorPos(pos.x, screenmetrics::height(rect) - iparams::get_i("screen_pos_buf")) ;
    }
    void Jump2Bottom::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Jump2Bottom::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //Jump2XCenter
    Jump2XCenter::Jump2XCenter()
    : BindedFuncCreator("jump_to_xcenter")
    {}
    void Jump2XCenter::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;
        SetCursorPos(screenmetrics::width(rect) / 2, pos.y) ;
    }
    void Jump2XCenter::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Jump2XCenter::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //Jump2YCenter
    Jump2YCenter::Jump2YCenter()
    : BindedFuncCreator("jump_to_ycenter")
    {}
    void Jump2YCenter::sprocess() {
        POINT pos ;
        GetCursorPos(&pos) ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;
        SetCursorPos(pos.x, screenmetrics::height(rect) / 2) ;
    }
    void Jump2YCenter::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Jump2YCenter::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
