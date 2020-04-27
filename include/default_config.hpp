#ifndef _DEFAULT_CONFIG_HPP
#define _DEFAULT_CONFIG_HPP

#include "path.hpp"
#include "ini_parser.hpp"

namespace DefaultConfig
{
    template <typename T>
    inline const auto get_property(const std::string& key) {
        static const auto pt = INIParser::load_config(Path::CONFIG_INI()) ;
        return pt.get_optional<T>("Property." + key).get() ;
    }

    
    inline static const auto& SCREEN_POS_BUF() {
        static const auto obj = get_property<int>("screen_pos_buf") ;
        return obj ;
    }

    inline static const auto& MOVE_ACCELERATION() {
        static const auto obj =  get_property<int>("move_acceleration") ;
        return obj ;
    }
    inline static const auto& CURSOR_WEIGHT() {
        static const auto obj = get_property<int>("cursor_weight") ;
        return obj ;
    }

    inline static const auto& YSCROLL_SPEED() {
        static const auto obj = get_property<int>("yscroll_speed") ;
        return obj ;
    }
    inline static const auto& XSCROLL_SPEED() {
        static const auto obj = get_property<int>("xscroll_speed") ;
        return obj ;
    }
    inline static const auto& YSCROLL_SCREEN_RATIO() {
        static const auto obj = get_property<float>("yscroll_screen_ratio") ;
        return obj ;
    }
    inline static const auto& XSCROLL_SCREEN_RATIO() {
        static const auto obj = get_property<float>("xscroll_screen_ratio") ;
        return obj ;
    }
    inline static const auto& CMD_MAX_CHAR() {
        static const auto obj = get_property<std::size_t>("cmd_max_char") ;
        return obj ;
    }
}

#endif