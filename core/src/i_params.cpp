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
        jp.clear() ;
        std::ifstream ifs(Path::SETTINGS()) ;
        ifs >> jp ;
    }

    template <typename T>
    inline static const T _get(const std::string name) {
        return jp.at(name).at("value").get<T>() ;
    }

    const std::string get_s(const std::string name) {
        return _get<std::string>(name) ;
    }
    float get_f(const std::string name) {
        return _get<float>(name) ;
    }
    double get_d(const std::string name) {
        return _get<double>(name) ;
    }
    int get_i(const std::string name) {
        return _get<int>(name) ;
    }
    long get_l(const std::string name) {
        return _get<long>(name) ;
    }
    std::size_t get_z(const std::string name) {
        return _get<std::size_t>(name) ;
    }
    bool get_b(const std::string name) {
        return _get<bool>(name) ;
    }
    unsigned char get_uc(const std::string name) {
        return _get<unsigned char>(name) ;
    }
}
