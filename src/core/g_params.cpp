#include "g_params.hpp"

#include <fstream>
#include <unordered_map>

#if defined(DEBUG)
#include <iostream>
#endif

#include "util/disable_gcc_warning.hpp"

#include <nlohmann/json.hpp>

#include "util/enable_gcc_warning.hpp"

#include "path.hpp"


namespace
{
    std::unordered_map<std::string, std::string>  g_str_params{} ;
    std::unordered_map<std::string, bool> g_bool_params{} ;
    std::unordered_map<std::string, double> g_num_params{} ;

    using namespace nlohmann ;
    json g_default_params{} ;
}

namespace vind
{
    namespace core {
        void initialize_params() {
            g_default_params.clear() ;
            std::ifstream ifs(SETTINGS_DEFAULT()) ;
            ifs >> g_default_params ;
        }

        void reset_all_params() {
            g_str_params.clear() ;
            g_num_params.clear() ;

            for(auto& [name, obj] : g_default_params.items()) {
                try {
                    auto& val = obj.at("value") ;
                    if(val.is_boolean()) {
                        g_bool_params[name] = val ;
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
            return g_bool_params.at(name) != 0 ;
        }

        unsigned char get_uc(const std::string& name) {
            return static_cast<unsigned char>(g_num_params.at(name)) ;
        }

        void do_set(const std::string& name, const std::string& val) {
            g_str_params[name] = val ;
        }
        void do_set(const std::string& name, std::string&& val) {
            g_str_params[name] = std::move(val) ;
        }

        void do_set(const std::string& name, int val) {
            g_num_params[name] = val ;
        }
        void do_set(const std::string& name, double val) {
            g_num_params[name] = val ;
        }

        void do_set(const std::string& name, bool val) {
            g_bool_params[name] = val ;
        }


        ParamType validate_param_type(const std::string& name) {
            if(g_bool_params.find(name) != g_bool_params.end()) {
                return ParamType::BOOL ;
            }
            if(g_num_params.find(name) != g_num_params.end()) {
                return ParamType::NUMBER ;
            }
            if(g_str_params.find(name) != g_str_params.end()) {
                return ParamType::STRING ;
            }

            return ParamType::UNDEFINED ;
        }
    }
}
