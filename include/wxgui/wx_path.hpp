#ifndef _WX_PATH_HPP
#define _WX_PATH_HPP

#include "path.hpp"

namespace Path
{
    static constexpr auto DEFAULT_BINDS       = "admin_config/default.xml" ;
    static constexpr auto DEFAULT_PARAMS      = "admin_config/default.ini" ;
    static constexpr auto DEFAULT_OPTS_PARAMS = "admin_config/default_opts.ini" ;
    static constexpr auto DEFAULT_OPTS        = "admin_config/default_opts_bool.ini" ;
    static constexpr auto DEFAULT_EXAPPS      = "admin_config/default_exapp.ini" ;
    static constexpr auto DEFAULT_KBTYPE      = "admin_config/default_kbtype.pth" ;

    inline static const auto& UI_LANG_IDX() noexcept {
        static const auto obj = _get_path("custom_ui_language.idx") ;
        return obj ;
    }

    inline static const auto& ICON_IDX() noexcept {
        static const auto obj = _get_path("custom_icon.idx") ;
        return obj ;
    }

    static constexpr auto DEFAULT_UI_LANG_IDX  = "admin_config/default_ui_language.idx" ;
    static constexpr auto DEFAULT_ICON_IDX     = "admin_config/default_icon.idx" ;

    static constexpr auto DEFAULT_RESOLUTION_IDX = "admin_config/default_propdlg_resolution.idx" ;
    inline static const auto& RESOLUTION_IDX() noexcept {
        static const auto obj = _get_path("custom_propdlg_resolution.idx") ;
        return obj ;
    }
}

#endif