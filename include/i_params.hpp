#ifndef _I_PARAMS_HPP
#define _I_PARAMS_HPP

#include <string>

namespace iParams
{
    const std::string get_s(const std::string name) ;

    float get_f(const std::string name) ;
    double get_d(const std::string name) ;

    int get_i(const std::string name) ;
    long get_l(const std::string name) ;

    std::size_t get_z(const std::string name) ;
    bool get_b(const std::string name) ;

    void load_config() ;
} ;

#endif
