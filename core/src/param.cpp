#include "coreio/param.hpp"

#include <unordered_map>

namespace
{
    std::unordered_map<std::string, std::string>  g_str_params{} ;
    std::unordered_map<std::string, std::int32_t> g_num_params{} ;
}

namespace vind
{
    namespace param {
        std::string get_s(const std::string& name) {
            return g_str_params.at(name) ;
        }

        float get_f(const std::string& name) {
            return static_cast<float>(g_num_params.at(name)) ;
        }

        double get_d(const std::string& name) {
            return static_cast<double>(g_num_params.at(name)) ;
        }

        int get_i(const std::string& name) {
            return g_num_params.at(name) ;
        }

        long get_l(const std::string& name) {
            return g_num_params.at(name) ;
        }

        std::size_t get_z(const std::string& name) {
            return static_cast<std::size_t>(g_num_params.at(name)) ;
        }

        bool get_b(const std::string& name) {
            return g_num_params.at(name) != 0 ;
        }

        unsigned char get_uc(const std::string& name) {
            return static_cast<unsigned char>(g_num_params.at(name)) ;
        }

        void set(const std::string& name, const std::string& val) {
            g_str_params[name] = val ;
        }
        void set(const std::string& name, std::string&& val) {
            g_str_params[name] = std::move(val) ;
        }
        void set(const std::string& name, int val) {
            g_num_params[name] = val ;
        }
        void set(const std::string& name, bool val) {
            g_num_params[name] = !val ? 0 : 1 ;
        }
    }
}
