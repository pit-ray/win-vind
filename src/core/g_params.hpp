#ifndef _G_PARAMS_HPP
#define _G_PARAMS_HPP

#include <string>

namespace vind
{
    namespace core {
        void initialize_params() ;

        void reset_all_params() ;

        std::string get_s(const std::string& name) ;

        float get_f(const std::string& name) ;

        double get_d(const std::string& name) ;

        int get_i(const std::string& name) ;

        long get_l(const std::string& name) ;

        std::size_t get_z(const std::string& name) ;

        bool get_b(const std::string& name) ;

        unsigned char get_uc(const std::string& name) ;

        void do_set(const std::string& name, const std::string& val) ;
        void do_set(const std::string& name, std::string&& val) ;

        void do_set(const std::string& name, int val) ;
        void do_set(const std::string& name, double val) ;
        void do_set(const std::string& name, bool val) ;

        enum ParamType : unsigned char {
            STRING,
            NUMBER,
            BOOL,
            UNDEFINED
        } ;

        ParamType validate_param_type(const std::string& name) ;
    }
}
#endif
