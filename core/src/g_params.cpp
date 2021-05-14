#include "g_params.hpp"

#include <unordered_map>

namespace
{
    std::unordered_map<std::string, std::string>  g_str_params{} ;
    std::unordered_map<std::string, double> g_num_params{} ;
}

namespace vind
{
    namespace gparams {
        void clear() {
            g_str_params.clear() ;
            g_num_params.clear() ;
        }

        void remove(const std::string& name) {
            g_str_params.erase(name) ;
            g_num_params.erase(name) ;
        }

        std::string get_s(const std::string& name) {
            return g_str_params.at(name) ;
        }

        float get_f(const std::string& name) {
            return static_cast<float>(g_num_params.at(name)) ;
        }

        double get_d(const std::string& name) {
            return g_num_params.at(name) ;
        }

        int get_i(const std::string& name) {
            return static_cast<int>(g_num_params.at(name)) ;
        }

        long get_l(const std::string& name) {
            return static_cast<long>(g_num_params.at(name)) ;
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
        void set(const std::string& name, double val) {
            g_num_params[name] = val ;
        }
        void set(const std::string& name, bool val) {
            g_num_params[name] = !val ? 0 : 1 ;
        }
    }
}
