#ifndef _SIMPLE_TEXT_REGISTER_HPP
#define _SIMPLE_TEXT_REGISTER_HPP

namespace vind
{
    namespace simpletxreg {
        enum class RegType : unsigned char {
            None,
            Chars,
            Lines,
        } ;

        RegType get_register_type() noexcept ;
        void set_register_type(RegType t) noexcept ;
    }
}

#endif
