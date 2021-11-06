#include "simple_text_register.hpp"

namespace
{
    auto g_rgtype = vind::simpletxreg::RegType::None ;
}

namespace vind
{
    namespace simpletxreg {
        RegType get_register_type() noexcept {
            return g_rgtype ;
        }

        void set_register_type(RegType t) noexcept {
            g_rgtype = t ;
        }
    }
}
