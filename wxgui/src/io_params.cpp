#include "io_params.hpp"

#include <fstream>
#include <iomanip>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include "enable_gcc_warning.hpp"

#include "msg_logger.hpp"
#include "path.hpp"


namespace ioParams
{
    using json = nlohmann::json ;
    static json parser{} ;
    static json def_parser{} ;
    bool initialize() {
        try {
            std::ifstream ifs(Path::Default::SETTINGS()) ;
            ifs >> def_parser ;

            load_config() ;
            return true ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
            return false ;
        }
    }

    bool load_config() {
        parser.clear() ;
        try {
            std::ifstream ifs(Path::SETTINGS()) ;
            ifs >> parser ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
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
            ERROR_PRINT(e.what()) ;
            return false ;
        }
        return true ;
    }

    template <typename T1, typename T2>
    inline static const auto _get_v(const json& json_parser, T2&& path) noexcept {
        try {return json_parser.at(json::json_pointer("/" + path + "/value")).get<T1>() ;}
        catch(const std::exception& e) {
            ERROR_PRINT(std::string(e.what()) + ", not found " + path) ;
            return T1{0} ;
        }
    }
    const wxString get_vwxs(const std::string path) noexcept {
        return wxString::FromUTF8(_get_v<std::string>(parser, path)) ;
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
            ERROR_PRINT(std::string(e.what()) + ", not found " + path) ;
            return wxT("ERROR_PRINT") ;
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
                    ERROR_PRINT(std::string(e.what()) + "not found item") ;
                    continue ;
                }
            }

            return v ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(std::string(e.what()) + ", so failed loading choices (" + path + ")") ;
            return choices_t{} ;
        }
    }

    const choices_t get_choices(const std::string path) noexcept {
        return _get_choices(parser, path) ;
    }

    template <typename T>
    inline static const auto to_choices_wxstr(T&& choices) noexcept {
        choices_wxstr_t wxchoices ;
        for(const auto& c : choices) {
            item_wxstr_t wxitems ;
            for(const auto& i : c) {
                wxitems[i.first] = wxString::FromUTF8(i.second) ;
            }
            wxchoices.push_back(std::move(wxitems)) ;
        }
        return wxchoices ;
    }

    const choices_wxstr_t get_choices_wxstr(const std::string path) noexcept {
        return to_choices_wxstr(_get_choices(parser, path)) ;
    }

    namespace Default {
        const wxString get_vwxs(const std::string path) noexcept {
            return wxString::FromUTF8(_get_v<std::string>(def_parser, path)) ;
        }
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

        const choices_wxstr_t get_choices_wxstr(const std::string path) noexcept {
            return to_choices_wxstr(_get_choices(def_parser, path)) ;
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
            ERROR_PRINT(std::string(e.what()) + ", so failed loading choices (" + path + ")") ;
            return ;
        }
    }

    void set(const std::string path, const choices_wxstr_t& choices) noexcept {
        choices_t utf8_choices ;
        for(const auto& c : choices) {
            item_t utf8_items ;
            for(const auto& i : c) {
                utf8_items[i.first] = i.second.ToUTF8().data() ;
            }
            utf8_choices.push_back(std::move(utf8_items)) ;
        }
        set(path, utf8_choices) ;
    }

    template <typename T1, typename T2>
    inline static void _set(T1&& path, const T2 value) noexcept {
        try {parser.at(json::json_pointer("/" + path + "/value")) = value ;}
        catch(const std::exception& e) {
            ERROR_PRINT(std::string(e.what()) + ", failed to store value at " + path) ;
        }
    }

    void set(const std::string path, const wxString value) noexcept {
        _set(path, value.ToUTF8().data()) ;
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
