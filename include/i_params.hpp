#ifndef _I_PARAMS_HPP
#define _I_PARAMS_HPP

#include <string>

namespace iParams
{
    const std::string get_s(const std::string name) noexcept ;

    float get_f(const std::string name) noexcept ;
    double get_d(const std::string name) noexcept ;

    int get_i(const std::string name) noexcept ;
    long get_l(const std::string name) noexcept ;

    std::size_t get_z(const std::string name) noexcept ;
    bool get_b(const std::string name) noexcept ;

    void load_config() noexcept ;
} ;

#endif
