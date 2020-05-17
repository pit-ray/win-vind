#include "pref_parser.hpp"

#include "disable_gcc_warning.hpp"
#include <boost/property_tree/xml_parser.hpp>
#include "enable_gcc_warning.hpp"

#include "ini_parser.hpp"
#include "msg_logger.hpp"
#include "path.hpp"

namespace PrefParser
{
    static constexpr auto DEFAULT_BINDS  = "config/default.xml" ;
    static constexpr auto DEFAULT_PARAMS = "config/default.ini" ;
    static constexpr auto DEFAULT_OPTS   = "config/default_opts.ini" ;

    template <typename T>
    void catch_except(const T& e) {
        using namespace Logger ;
        error_stream << E << e.what() << ": cannot loaded xml. (PrefParser::_load_xml_core)\n" ;
    }

    template <typename T>
    inline static const auto _load_xml_core(T&& filename, const std::string tag) { //filename may be long string, so not applied SSO.
        using namespace boost::property_tree ;
        try {
            ums_str_t ssm ;
            ptree pt ;
            read_xml(std::forward<T>(filename), pt) ;

            for(const auto& pt_func : pt.get_child("config." + tag)) {
                try {
                    const auto& name = pt_func.second.get_optional<std::string>("<xmlattr>.name").get() ;
                    const auto& data = pt_func.second.data() ;
                    Logger::msg_stream << Logger::M << "[config." << tag << "] " << name << data << std::endl ;
                    ssm[name] = data ;
                }
                catch(ptree_bad_data& e) {
                    catch_except(e) ;
                    continue ;
                }
            }

            return ssm ;
        }
        catch(ptree_bad_path& e) {
            catch_except(e) ;
            return ums_str_t{} ;
        }
        catch(xml_parser_error& e) {
            catch_except(e) ;
            return ums_str_t{} ;
        }
    }

    const ums_ums_str_t load_binds() {
        ums_ums_str_t map ;
        map["binds"] = _load_xml_core(Path::CONFIG_XML(), "binds") ;
        map["commands"] = _load_xml_core(Path::CONFIG_XML(), "commands") ;
        return map ;
    }

    const ums_ums_str_t& load_default_binds() {
        static ums_ums_str_t map ;
        if(map.empty()) {
            map["binds"] = _load_xml_core(DEFAULT_BINDS, "binds") ;
            map["commands"] = _load_xml_core(DEFAULT_BINDS, "commands") ;
        }
        return map ;
    }

    void save_binds(const ums_ums_str_t& maps) {
        using namespace boost::property_tree ;
        try {
            ptree pt ;

            auto child_c = pt.add("config.commands", "") ;

            for(const auto& p : maps.at("binds")) {
                auto& child = pt.add("config.binds.func", p.second) ;
                child.put("<xmlattr>.name", p.first) ;
            }

            for(const auto& p : maps.at("commands")) {
                auto& child = pt.add("config.commands.cmd", p.second) ;
                child.put("<xmlattr>.name", p.first) ;
            }

            using namespace boost::property_tree::xml_parser ;
            write_xml(Path::CONFIG_XML(), pt, std::locale(),
                xml_writer_make_settings(' ', 2, widen<std::string>("utf-8"))) ;
        }
        catch(ptree_bad_path& e) {
            catch_except(e) ;
            return ;
        }
        catch(xml_parser_error& e) {
            catch_except(e) ;
            return ;
        }
        catch(std::out_of_range& e) {
            catch_except(e) ;
            return ;
        }
    }


    template <typename T>
    inline static const auto _load_ini_core(T&& filename, const std::string sect) {
        const auto pt = INIParser::load_config(std::forward<T>(filename)) ;

        ums_str_t ssm{} ;

        using namespace boost::property_tree ;
        for(const auto& item : pt.get_child(sect, ptree())) {
            const auto& keys  = item.first ;
            const auto& value = item.second.template get_value<std::string>() ;

            ssm[keys] = value ;
        }

        return ssm ;
    }

    template <typename T1, typename T2>
    inline static void _save_ini_core(T1&& params, T2&& filename, const std::string sect) {
        using namespace boost::property_tree ;
        ptree pt ;

        for(const auto& p : std::forward<T1>(params)) {
            pt.put(sect + "." + p.first, p.second) ;
        }

        write_ini(std::forward<T2>(filename), pt) ;
    }

    const ums_str_t load_params() {
        return _load_ini_core(Path::CONFIG_INI(), "Property") ;
    }

    const ums_str_t& load_default_params() {
        static auto cache = _load_ini_core(DEFAULT_PARAMS, "Property") ;
        return cache ;
    }

    void save_params(const ums_str_t& params) {
        _save_ini_core(params, Path::CONFIG_INI(), "Property") ;
    }

    template <typename T>
    inline static auto _ums_str2bool(T&& filename, const std::string sect) {
        ums_bool_t sbm{} ;
        for(const auto& p : _load_ini_core(std::forward<T>(filename), std::move(sect))) {
            sbm[p.first] = p.second == "0" ? false : true ;
        }
        return sbm ;
    }

    const ums_bool_t load_opts() {
        return _ums_str2bool(Path::CONFIG_OPTION_INI(), "Options") ;
    }

    const ums_bool_t& load_default_opts() {
        static auto cache = _ums_str2bool(DEFAULT_OPTS, "Options") ;
        return cache ;
    }

    void save_opts(const ums_bool_t& opts) {
        ums_str_t ssm{} ;

        for(const auto& op : opts) {
            ssm[op.first] = op.second ? "1" : "0" ;
        }

        _save_ini_core(std::move(ssm), Path::CONFIG_OPTION_INI(), "Options") ;
    }
}