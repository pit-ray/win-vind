#include "io_params.hpp"

#include <fstream>
#include <iomanip>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "msg_logger.hpp"
#include "path.hpp"


namespace ioParams
{
    using namespace nlohmann ;
    json parser ;

    inline static const auto _init() {
        try {
            json j ;
            std::ifstream ifs(Path::Default::SETTINGS()) ;
            ifs >> j ;
            return j ;
        }
        catch(const std::exception& e) {
            ERROR_STREAM << e.what() << "(ioParams::_init)\n" ;
            return json() ;
        }
    }
    static const json def_parser = _init() ;

    bool load_config() {
        parser.clear() ;
        try {
            std::ifstream ifs(Path::SETTINGS()) ;
            ifs >> parser ;
        }
        catch(const std::exception& e) {
            ERROR_STREAM << e.what() << "(ioParams::load_config)\n" ;
            return false ;
        }
        return true ;
    }

    bool save_config() {
        try {
            std::ofstream ofs(Path::SETTINGS()) ;
            ofs << std::setw(4) << parser << std::endl ;
        }
        catch(const std::exception& e) {
            ERROR_STREAM << e.what() << "(ioParams::save_config)\n" ;
            return false ;
        }
        return true ;
    }

    template <typename T1, typename T2>
    inline static const auto _get_v(const json& json_parser, T2&& path) noexcept {
        try {return json_parser.at(json::json_pointer("/" + path + "/value")).get<T1>() ;}
        catch(const std::exception& e) {
            ERROR_STREAM << e.what() << ", not found " << path << " (ioParams::_get_v)\n" ;
            return T1{0} ;
        }
    }

    const std::string get_vs(const std::string path) noexcept {
        return _get_v<std::string>(parser, path) ;
    }

    int get_vi(const std::string path) noexcept {
        return _get_v<int>(parser, path) ;
    }

    double get_vd(const std::string path) noexcept {
        return _get_v<double>(parser, path) ;
    }

    bool get_vb(const std::string path) noexcept {
        return _get_v<bool>(parser, path) ;
    }

    const wxString get_label(const std::string path) noexcept {
        try {return wxString::FromUTF8(parser.at(json::json_pointer("/" + path + "/" + get_vs("ui_lang"))).get<std::string>().c_str()) ;}
        catch(const std::exception& e) {
            ERROR_STREAM << e.what() << ", not found " << path << " (ioParams::get_label)\n" ;
            return wxT("ERROR") ;
        }
    }

    template <typename T>
    inline static const auto _get_choices(const json& json_parser, T&& path) noexcept {
        try {
            choices_t v ;
            const auto& choices = json_parser.at(json::json_pointer("/" + path + "/choices")) ;

            for(const auto& item : choices) {
                try {
                    item_t i ;
                    for(const auto& e : item.items()) {
                        i.insert(std::make_pair(e.key(), e.value().get<std::string>())) ;
                    }
                    v.push_back(std::move(i)) ;
                }
                catch(const std::exception& e) {
                    ERROR_STREAM << e.what() << "not found item (ioParams::_get_choices)\n" ;
                    continue ;
                }
            }

            return v ;
        }
        catch(const std::exception& e) {
            ERROR_STREAM << "failed loading choices (" << path << ") (ioParams::get__choices)\n" ;
            return choices_t{} ;
        }
    }

    const choices_t get_choices(const std::string path) noexcept {
        return _get_choices(parser, path) ;
    }

    namespace Default {
        const std::string get_vs(const std::string path) noexcept {
            return _get_v<std::string>(def_parser, path) ;
        }

        int get_vi(const std::string path) noexcept {
            return _get_v<int>(def_parser, path) ;
        }

        double get_vd(const std::string path) noexcept {
            return _get_v<double>(def_parser, path) ;
        }

        bool get_vb(const std::string path) noexcept {
            return _get_v<bool>(def_parser, path) ;
        }

        const choices_t get_choices(const std::string path) noexcept {
            return _get_choices(def_parser, path) ;
        }
    }

    void set(const std::string path, const choices_t& choices) noexcept {
        try {
            json ar ;
            for(const auto& item : choices) {
                json j ;
                for(const auto& e : item) j[e.first] = e.second ;
                ar.push_back(std::move(j)) ;
            }
            auto& node = parser.at(json::json_pointer("/" + path + "/choices")) ;
            node.clear() ;
            node = ar ;
        }
        catch(const std::exception& e) {
            ERROR_STREAM << "failed loading choices (" << path << ") (ioParams::set)\n" ;
            return ;
        }
    }

    template <typename T1, typename T2>
    inline static void _set(T1&& path, const T2 value) noexcept {
        try {parser.at(json::json_pointer("/" + path + "/value")) = value ;}
        catch(const std::exception& e) {
            ERROR_STREAM << e.what() << ", failed to store value at " << path << " (ioParams::_set)\n" ;
        }
    }

    void set(const std::string path, const wxString value) noexcept {
        _set(path, value.ToStdString()) ;
    }
    void set(const std::string path, const std::string value) noexcept {
        _set(path, value) ;
    }
    void set(const std::string path, const int value) noexcept {
        _set(path, value) ;
    }
    void set(const std::string path, const long value) noexcept {
        _set(path, value) ;
    }
    void set(const std::string path, const double value) noexcept {
        _set(path, value) ;
    }
    void set(const std::string path, const float value) noexcept {
        _set(path, value) ;
    }
    void set(const std::string path, const bool value) noexcept {
        _set(path, value) ;
    }
}