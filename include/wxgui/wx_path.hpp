#ifndef _WX_PATH_HPP
#define _WX_PATH_HPP

namespace Path
{
    static constexpr auto DEFAULT_BINDS       = "config/default.xml" ;
    static constexpr auto DEFAULT_PARAMS      = "config/default.ini" ;
    static constexpr auto DEFAULT_OPTS_PARAMS = "config/default_opts.ini" ;
    static constexpr auto DEFAULT_OPTS        = "config/default_opts_bool.ini" ;
    static constexpr auto DEFAULT_EXAPPS      = "config/default_exapp.ini" ;
    static constexpr auto DEFAULT_KBTYPE      = "config/default_kbtype.pth" ;

    static constexpr auto UI_LANG_PTH = "config/custom_ui_language.pth" ;
    static constexpr auto ICON_PTH    = "config/custom_icon.pth" ;

    static constexpr auto DEFAULT_UI_LANG_PTH  = "config/default_ui_language.pth" ;
    static constexpr auto DEFAULT_ICON_PTH     = "config/default_icon.pth" ;

    inline static const auto& ICON_PATH() noexcept {
        static const auto get_file_name = [] {
            std::ifstream ifs{ICON_PTH} ;
            std::string filename{} ;
            std::getline(ifs, filename) ;
            return filename ;
        } ;

        static const auto obj = get_file_name();
        return obj ;
    }
}

#endif