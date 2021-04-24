#include "bind/uia/easy_click.hpp"

#include "bind/base/ntype_logger.hpp"
#include "bind/uia/ec_core.hpp"
#include "bind/uia/ec_scan.hpp"
#include "util/def.hpp"


namespace
{
    using namespace vind::easyclick ;
    bool g_is_initialized = false ;
    void init_ec() {
        if(!g_is_initialized) {
            initialize_global_uia() ;
            g_is_initialized = true ;
        }
    }
}

namespace vind
{
    using namespace easyclick ;

    //EasyClickLeft
    EasyClickLeft::EasyClickLeft()
    : BindedFuncCreator("easy_click_left")
    {
        init_ec() ;
    }
    void EasyClickLeft::sprocess() {
        do_easy_click(KEYCODE_MOUSE_LEFT) ;
    }
    void EasyClickLeft::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EasyClickRight
    EasyClickRight::EasyClickRight()
    : BindedFuncCreator("easy_click_right")
    {
        init_ec() ;
    }
    void EasyClickRight::sprocess() {
        do_easy_click(KEYCODE_MOUSE_RIGHT) ;
    }
    void EasyClickRight::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickRight::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EasyClickMid
    EasyClickMid::EasyClickMid()
    : BindedFuncCreator("easy_click_mid")
    {
        init_ec() ;
    }
    void EasyClickMid::sprocess() {
        do_easy_click(KEYCODE_MOUSE_MID) ;
    }
    void EasyClickMid::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickMid::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //EasyClickHover
    EasyClickHover::EasyClickHover()
    : BindedFuncCreator("easy_click_hover")
    {
        init_ec() ;
    }
    void EasyClickHover::sprocess() {
        do_easy_click(KEYCODE_UNDEFINED) ;
    }
    void EasyClickHover::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EasyClickHover::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
