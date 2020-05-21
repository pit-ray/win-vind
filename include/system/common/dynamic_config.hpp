#ifndef _DYNAMIC_CONFIG_HPP
#define _DYNAMIC_CONFIG_HPP

#include <unordered_map>

#include "path.hpp"
#include "ini_parser.hpp"

class DynamicConfig
{
private:
    static std::unordered_map<std::string, float> sfm ;
    static float default_value ;

    static const float& get_config(const std::string&& key) {
        if(sfm.empty()) {
            load_config() ;
        }
        try {
            return sfm.at(std::move(key)) ;
        }
        catch(std::out_of_range& e) {
            return default_value ;
        }
    }

public:
    static void load_config() {
        const auto pt = INIParser::load_config(Path::CONFIG_INI()) ;
        for(const auto& item : pt.get_child("Property", boost::property_tree::ptree())) {
            const auto& key = item.first ;
            const auto& value = item.second.template get_value<float>() ;
            if(!key.empty()) {
                sfm[key] = value ;
            }
        }
    }

    static const auto SCREEN_POS_BUF() {
        return static_cast<int>(get_config("screen_pos_buf")) ;
    }
    static const auto& MOVE_ACCELERATION() {
        return get_config("move_acceleration") ;
    }
    static const auto& CURSOR_WEIGHT() {
        return get_config("cursor_weight") ;
    }
    static const auto& YSCROLL_SPEED() {
        return get_config("yscroll_speed") ;
    }
    static const auto& XSCROLL_SPEED() {
        return get_config("xscroll_speed") ;
    }
    static const auto& YSCROLL_SCREEN_RATIO() {
        return get_config("yscroll_screen_ratio") ;
    }
    static const auto& XSCROLL_SCREEN_RATIO() {
        return get_config("xscroll_screen_ratio") ;
    }
    static const auto CMD_MAX_CHAR() {
        return static_cast<std::size_t>(get_config("cmd_max_char")) ;
    }
} ;

#endif