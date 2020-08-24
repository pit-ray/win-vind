#include "pref_parser.hpp"

#include <fstream>

#include "disable_gcc_warning.hpp"
#include "json.hpp" //nlohmann (libs)
#include <boost/property_tree/xml_parser.hpp>
#include "enable_gcc_warning.hpp"

#include "ini_parser.hpp"
#include "msg_logger.hpp"
#include "path.hpp"
#include "wx_path.hpp"

namespace PrefParser
{
    template <typename T>
    void catch_except(const T& e) {
        ERROR_STREAM << e.what() << ": cannot loaded xml. (PrefParser::_load_xml_core)\n" ;
    }

    //filename may be long strings, so not applied SSO.
    template <typename T>
    inline static const auto _load_xml_core(T&& filename, const std::string tag) {
        using namespace boost::property_tree ;
        try {
            ums_str_t ssm ;
            ptree pt ;
            read_xml(std::forward<T>(filename), pt) ;

            for(const auto& pt_func : pt.get_child("config." + tag)) {
                try {
                    const auto& name = pt_func.second.get_optional<std::string>("<xmlattr>.name").get() ;
                    const auto& data = pt_func.second.data() ;
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

    //filename may be long strings, so not applied SSO.
    template <typename T>
    inline static const auto _load_bindings_core(T&& filename, const std::string tag) {
        try {
            ums_str_t ssm ;
            std::ifstream ifs(std::forward<T>(filename)) ;
            nlohmann::json jp{} ;

        /*
            for(const auto& obj : jp) {
                try {
                    const auto& data = pt_f ;
                    ssm[obj["name"]] = data ;
                }
                catch(ptree_bad_data& e) {
                    catch_except(e) ;
                    continue ;
                }
            }
        */

            return ssm ;
        }
        catch(const nlohmann::json::exception& e) {
            catch_except(e) ;
            return ums_str_t{} ;
        }
    }

    const ums_ums_str_t load_binds() {
        ums_ums_str_t map ;
        map["binds"] = _load_xml_core(Path::CONFIG_XML(), "binds") ;
        map["commands"] = _load_xml_core(Path::CONFIG_XML(), "commands") ;
        return map ; //apply RVO
    }

    const ums_ums_str_t& load_default_binds() {
        auto load = []() {
            static ums_ums_str_t map ;
            map["binds"] = _load_xml_core(Path::DEFAULT_BINDS, "binds") ;
            map["commands"] = _load_xml_core(Path::DEFAULT_BINDS, "commands") ;
            return map ;
        } ;

        static auto cache = load() ;
        return cache ;
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
            write_xml(Path::CONFIG_XML(), std::move(pt), std::locale(),
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
            const auto& key  = item.first ;
            const auto& value = item.second.template get_value<std::string>() ;

            ssm[key] = value ;
        }

        return ssm ; // apply RVO
    }

    template <typename T1, typename T2>
    inline static void _save_ini_core(T1&& params, T2&& filename, const std::string sect) {
        using namespace boost::property_tree ;
        ptree pt ;

        for(const auto& p : std::forward<T1>(params)) {
            pt.put(sect + "." + p.first, p.second) ;
        }

        write_ini(std::forward<T2>(filename), std::move(pt)) ;
    }

    const ums_str_t load_params() {
        return _load_ini_core(Path::CONFIG_INI(), "Property") ;
    }
    const ums_str_t& load_default_params() {
        static auto cache = _load_ini_core(Path::DEFAULT_PARAMS, "Property") ;
        return cache ;
    }
    void save_params(const ums_str_t& params) {
        _save_ini_core(params, Path::CONFIG_INI(), "Property") ;
    }

    const ums_str_t load_opts_params() {
        return _load_ini_core(Path::CONFIG_OPTS_INI(), "Property") ;
    }
    const ums_str_t& load_default_opts_params() {
        static auto cache = _load_ini_core(Path::DEFAULT_OPTS_PARAMS, "Property") ;
        return cache ;
    }
    void save_opts_params(const ums_str_t& params) {
        _save_ini_core(params, Path::CONFIG_OPTS_INI(), "Property") ;
    }

    template <typename T>
    inline static auto _ums_str2bool(T&& filename, const std::string sect) {
        ums_bool_t sbm{} ;
        for(const auto& p : _load_ini_core(std::forward<T>(filename), std::move(sect))) {
            sbm[p.first] = p.second == "0" ? false : true ;
        }
        return sbm ; //apply RVO
    }

    const ums_bool_t load_opts_bool() {
        return _ums_str2bool(Path::CONFIG_OPTS_BOOL_INI(), "Options") ;
    }
    const ums_bool_t& load_default_opts_bool() {
        static auto cache = _ums_str2bool(Path::DEFAULT_OPTS, "Options") ;
        return cache ;
    }
    void save_opts_bool(const ums_bool_t& opts) {
        ums_str_t ssm{} ;

        for(const auto& op : opts) {
            ssm[op.first] = op.second ? "1" : "0" ;
        }

        _save_ini_core(std::move(ssm), Path::CONFIG_OPTS_BOOL_INI(), "Options") ;
    }

    //exapps
    const ums_str_t load_exapps() {
        return _load_ini_core(Path::CONFIG_EXAPP_INI(), "ExAppPath") ;
    }
    const ums_str_t& load_default_exapps() {
        static auto cache = _load_ini_core(Path::DEFAULT_EXAPPS, "ExAppPath") ;
        return cache ;
    }
    void save_exapps(const ums_str_t& exapps) {
        _save_ini_core(exapps, Path::CONFIG_EXAPP_INI(), "ExAppPath") ;
    }

    template <typename T>
    inline static const auto _load_pth(T&& filename) {
        std::ifstream ifs{std::forward<T>(filename)} ;
        std::string type ;
        std::getline(ifs, type) ;
        return type ;
    }
    template <typename T1, typename T2>
    inline static void _save_pth(T1&& filename, T2&& path) {
        std::ofstream ofs{std::forward<T1>(filename), std::ios::trunc} ;
        ofs << std::forward<T2>(path) ;
    }

    //kbtype
    const std::string load_kbtype() {
        return _load_pth(Path::KBTYPE_PTH()) ;
    }
    const std::string load_default_kbtype() {
        static auto cache = _load_pth(Path::DEFAULT_KBTYPE) ;
        return cache ;
    }
    void save_kbtype(const std::string& path) {
        _save_pth(Path::KBTYPE_PTH(), path) ;
    }
    void save_kbtype(std::string&& path) {
        _save_pth(Path::KBTYPE_PTH(), std::move(path)) ;
    }

    template <typename T1>
    inline static const auto s2e(const std::string str) { //apply SSO
        return static_cast<T1>(std::stoi(str)) ;
    }

    template <typename T>
    inline static const auto e2s(const T idx) {
        return std::to_string(static_cast<int>(idx)) ;
    }

    //lang
    wxGUI::Language load_uilang() {
        return s2e<wxGUI::Language>(_load_pth(Path::UI_LANG_IDX())) ;
    }
    const wxGUI::Language& load_default_uilang() {
        static auto cache = s2e<wxGUI::Language>(_load_pth(Path::DEFAULT_UI_LANG_IDX)) ;
        return cache ;
    }
    void save_uilang(const wxGUI::Language idx) {
        _save_pth(Path::UI_LANG_IDX(), e2s(idx)) ;
    }

    //system tray icon
    wxGUI::IconStyle load_sticon() {
        return s2e<wxGUI::IconStyle>(_load_pth(Path::ICON_IDX())) ;
    }
    const wxGUI::IconStyle& load_default_sticon() {
        static auto cache = s2e<wxGUI::IconStyle>(_load_pth(Path::DEFAULT_ICON_IDX)) ;
        return cache ;
    }
    void save_sticon(const wxGUI::IconStyle idx) {
        _save_pth(Path::ICON_IDX(), e2s(idx)) ;
    }

    //config window's resolution PPI magnification
    unsigned char load_propdlg_res() {
        return static_cast<unsigned char>(std::stoi(_load_pth(Path::RESOLUTION_IDX()))) ;
    }
    unsigned char load_default_propdlg_res() {
        static auto cache =  static_cast<unsigned char>(std::stoi(_load_pth(Path::DEFAULT_RESOLUTION_IDX))) ;
        return cache ;
    }
    void save_propdlg_res(const unsigned char resolution_index) {
        _save_pth(Path::RESOLUTION_IDX(), std::to_string(resolution_index)) ;
    }
}