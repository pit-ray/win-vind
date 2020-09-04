#include "i_params.hpp"

#include <fstream>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "path.hpp"
#include "msg_logger.hpp"

namespace iParams
{
    static nlohmann::json jp ;
    void load_config() {
        try {
            jp.clear() ;
            std::ifstream ifs(Path::SETTINGS()) ;
            ifs >> jp ;
        }
        catch(const std::exception& e) {
            ERROR_STREAM << e.what() << " failed loading config for system (Params::load_config)\n" ;
            return ;
        }
    }

    template <typename T>
    inline static const T _get(const std::string name) noexcept {
        try {return jp.at(name).at("value").get<T>() ;}
        catch(const std::exception& e) {
            ERROR_STREAM << e.what() << " bad index (iParams::_get)\n" ;
            return T{0} ;
        }
    }

    const std::string get_s(const std::string name) noexcept {
        return _get<std::string>(name) ;
    }
    float get_f(const std::string name) noexcept {
        return _get<float>(name) ;
    }
    double get_d(const std::string name) noexcept {
        return _get<double>(name) ;
    }
    int get_i(const std::string name) noexcept {
        return _get<int>(name) ;
    }
    long get_l(const std::string name) noexcept {
        return _get<long>(name) ;
    }
    std::size_t get_z(const std::string name) noexcept {
        return _get<std::size_t>(name) ;
    }
    bool get_b(const std::string name) noexcept {
        return _get<bool>(name) ;
    }
}