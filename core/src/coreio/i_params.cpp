#include "coreio/err_logger.hpp"
#include "coreio/i_params.hpp"
#include "coreio/path.hpp"

#include <fstream>

#include "disable_gcc_warning.hpp"

#include <nlohmann/json.hpp>

#include "enable_gcc_warning.hpp"


//internal linkage
namespace
{
    nlohmann::json jp ;

    template <typename T>
    inline const T get_param(const std::string name) {
        return jp.at(name).at("value").get<T>() ;
    }
}

namespace vind
{
    namespace iparams {
        void load_config() {
            jp.clear() ;
            std::ifstream ifs(path::to_u8path(path::SETTINGS())) ;
            ifs >> jp ;
        }

        const std::string get_s(const std::string name) {
            return get_param<std::string>(name) ;
        }
        float get_f(const std::string name) {
            return get_param<float>(name) ;
        }
        double get_d(const std::string name) {
            return get_param<double>(name) ;
        }
        int get_i(const std::string name) {
            return get_param<int>(name) ;
        }
        long get_l(const std::string name) {
            return get_param<long>(name) ;
        }
        std::size_t get_z(const std::string name) {
            return get_param<std::size_t>(name) ;
        }
        bool get_b(const std::string name) {
            return get_param<bool>(name) ;
        }
        unsigned char get_uc(const std::string name) {
            return get_param<unsigned char>(name) ;
        }
    }
}
