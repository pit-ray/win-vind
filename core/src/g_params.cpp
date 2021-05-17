#include "g_params.hpp"

#include <fstream>
#include <iostream>
#include <unordered_map>


#include "disable_gcc_warning.hpp"

#include <nlohmann/json.hpp>

#include "enable_gcc_warning.hpp"

#include "path.hpp"


namespace
{
    std::unordered_map<std::string, std::string>  g_str_params{} ;
    std::unordered_map<std::string, double> g_num_params{} ;

    using namespace nlohmann ;
    json g_default_params{} ;
}

namespace vind
{
    namespace gparams {
        void initialize() {
            g_default_params.clear() ;
            std::ifstream ifs(path::to_u8path(path::Default::SETTINGS())) ;
            ifs >> g_default_params ;
        }

        void reset() {
            g_str_params.clear() ;
            g_num_params.clear() ;

            for(auto& [name, obj] : g_default_params.items()) {
                try {
                    auto& val = obj.at("value") ;
                    if(val.is_boolean()) {
                        g_num_params[name] = val ? 1 : 0 ;
                    }
                    else if(val.is_number()) {
                        g_num_params[name] = val.get<double>() ;
                    }
                    else {
                        g_str_params[name] = val.get<std::string>() ;
                    }
                }
                catch(const json::out_of_range&) {
                    continue ;
                }
            }
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
