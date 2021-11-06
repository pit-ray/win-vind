#ifndef _G_PARAMS_HPP
#define _G_PARAMS_HPP

#include <string>

namespace vind
{
    namespace gparams {
        void initialize() ;

        void reset() ;

        std::string get_s(const std::string& name) ;

        float get_f(const std::string& name) ;

        double get_d(const std::string& name) ;

        int get_i(const std::string& name) ;

        long get_l(const std::string& name) ;

        std::size_t get_z(const std::string& name) ;

        bool get_b(const std::string& name) ;

        unsigned char get_uc(const std::string& name) ;

        void set(const std::string& name, const std::string& val) ;
        void set(const std::string& name, std::string&& val) ;

        void set(const std::string& name, int val) ;
        void set(const std::string& name, double val) ;
        void set(const std::string& name, bool val) ;

        enum ValueType : unsigned char {
            STRING,
            NUMBER,
            BOOL,
            UNDEFINED
        } ;

        ValueType get_type(const std::string& name) ;
    }
}
#endif
