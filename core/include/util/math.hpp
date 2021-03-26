#ifndef _MATH_HPP
#define _MATH_HPP

namespace vind
{
    namespace util {
        inline constexpr unsigned int pow_i(int base, unsigned int exp) noexcept {
            return (exp == 0 ? 1 : base * pow_i(base, exp - 1)) ;
        }
        inline constexpr float pow_f(int base, int exp) noexcept {
            if(exp < 0) return pow_f(base, exp + 1) / base ;
            return (exp == 0 ? 1.0f : base * pow_f(base, exp - 1)) ;
        }
        inline constexpr double pow_d(int base, int exp) noexcept {
            if(exp < 0) return pow_d(base, exp + 1) / base ;
            return (exp == 0 ? 1.0 : base * pow_d(base, exp - 1)) ;
        }
    }
}
#endif
