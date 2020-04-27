#ifndef _INI_PARSER_HPP
#define _INI_PARSER_HPP

#include "disable_gcc_warning.hpp"
#include <boost/property_tree/ini_parser.hpp>
#include "enable_gcc_warning.hpp"

#include "msg_logger.hpp"

#include <string>

namespace INIParser
{
    template <typename T>
    inline static void catch_boost_except(T& e) {
        Logger::error_stream << "[Error] " << e.what() << " (default_config_core.cpp)\n" ;
    }

    inline const auto load_config(const std::string& filename) noexcept {
        using namespace boost::property_tree ;
        ptree pt ;
        try {
            read_ini(filename, pt) ;
            return std::move(pt) ;
        }
        catch(ini_parser_error& e) {
            catch_boost_except(e) ;
            return std::move(pt) ;
        }
        catch(ptree_bad_path& e) {
            catch_boost_except(e) ;
            return std::move(pt) ;
        }
        catch(ptree_bad_data& e) {
            catch_boost_except(e) ;
            return std::move(pt) ;
        }
    }
}

#endif